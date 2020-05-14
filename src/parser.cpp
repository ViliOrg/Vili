#include <vili/node.hpp>
#include <vili/types.hpp>

#include <fstream>
#include <iostream>
#include <stack>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/trace.hpp>

namespace peg = tao::pegtl;
using namespace std::string_literals;

namespace vili::parser
{
    namespace rules
    {
        // clang-format off
        struct identifier : peg::identifier {};
        struct indent : peg::seq<peg::bol, peg::star<peg::blank>> {};
        struct affectation : peg::seq<identifier, peg::pad<peg::one<':'>, peg::blank>> {};

        // String
        struct string_content : peg::star<peg::not_one<'"'>> {};
        struct string : peg::seq<peg::one<'"'>, string_content, peg::one<'"'>> {};

        // Booleans
        struct true_ : peg::string<'t', 'r', 'u', 'e'> {};
        struct false_ : peg::string<'f', 'a', 'l', 's', 'e'> {};
        struct boolean : peg::sor<true_, false_> {};

        // Numbers
        struct sign : peg::one<'+', '-'> {};
        struct integer : peg::seq<peg::opt<sign>, peg::plus<peg::digit>> {};
        struct number : peg::seq<peg::opt<sign>, peg::star<peg::digit>, peg::one<'.'>, peg::plus<peg::digit>> {};

        // Data
        struct data : peg::sor<boolean, number, integer, string> {};
        struct brace_based_object;
        struct array;
        struct object;
        struct template_usage;
        struct inline_element : peg::sor<boolean, number, integer, string, array, brace_based_object, template_usage> {};
        struct inline_node : peg::seq<affectation, inline_element> {};
        struct element : peg::sor<data, array, object, template_usage> {};

        // Arrays
        struct array_elements : peg::list<inline_element, peg::one<','>, peg::space> {};
        struct open_array : peg::one<'['> {};
        struct close_array : peg::one<']'> {};
        struct array : peg::seq<open_array, peg::pad_opt<array_elements, peg::space>, close_array> {};

        // Objects
        struct node;
        struct indent_based_object : peg::eol {};
        struct open_object : peg::one<'{'> {};
        struct close_object : peg::one<'}'> {};
        struct comma_or_newline : peg::seq<peg::pad<peg::sor<peg::one<','>, peg::eol>, peg::space>, peg::star<peg::space>> {};
        struct object_elements : peg::list<inline_node, comma_or_newline> {};
        struct brace_based_object : peg::seq<open_object, peg::pad_opt<object_elements, peg::space>, close_object> {};
        struct object : peg::sor<brace_based_object, indent_based_object> {};

        // Comments
        struct inline_comment : peg::seq<peg::one<'#'>, peg::until<peg::eol, peg::any>> {};

        // Templates
        struct template_keyword : peg::string<'t', 'e', 'm', 'p', 'l', 'a', 't', 'e'> {};
        struct template_decl : peg::seq<peg::bol, template_keyword, peg::blank, affectation, data> {}; // TODO: Include complex types
        struct template_usage : peg::identifier {};

        // Nodes
        struct node : peg::seq<affectation, element> {};
        struct full_node : peg::seq<indent, node, peg::opt<peg::eol>> {};
        struct empty_line : peg::seq<peg::star<peg::blank>, peg::eol> {};
        struct line : peg::sor<empty_line, inline_comment, template_decl, full_node> {};
        struct grammar : peg::until<peg::eof, peg::must<line>> {};
        // clang-format on
    }

    class node_in_stack
    {
    public:
        node* item;
        int indent;

        node_in_stack(node* node, int indent)
        {
            this->item = node;
            this->indent = indent;
        }
    };

    class state
    {
    private:
        std::string m_identifier;
        std::stack<node_in_stack> m_stack;
        int64_t m_indent_base = 4;
        int64_t m_indent_current = -1;
        std::unordered_map<std::string, node> m_templates;

    public:
        node root;
        state();
        void set_indent(int64_t indent);
        void use_indent();
        void set_active_identifier(const std::string& identifier);
        void open_block();
        void close_block();
        void push(node data);
        void push_template();
        [[nodiscard]] node get_template(const std::string& template_name) const;
    };

    state::state()
        : root(object {})
    {
        m_stack.emplace(&root, 0);
    }

    void state::set_indent(int64_t indent)
    {
        if (m_indent_current == -1 && indent > 0)
        {
            m_indent_base = indent;
        }
        if (indent % m_indent_base && m_stack.top().indent)
        {
            throw exceptions::inconsistent_indentation(indent, m_indent_base, EXC_INFO);
        }
        indent /= m_indent_base; // Normalize indentation to "levels"
        if (m_indent_current > indent)
        {
            for (auto decrease_indent = m_indent_current; decrease_indent > indent;
                 decrease_indent--)
            {
                this->close_block();
            }
        }
        else if (m_indent_current == indent && indent < m_stack.top().indent)
        {
            this->close_block();
        }
        else if (m_indent_current < indent)
        {
            if (m_indent_current - indent > 1)
            {
                throw exceptions::too_much_indentation(indent, EXC_INFO);
            }
        }
        m_indent_current = indent;
    }

    void state::use_indent()
    {
        m_stack.top().indent = m_indent_current + 1;
    }

    void state::set_active_identifier(const std::string& identifier)
    {
        m_identifier = identifier;
    }

    void state::open_block()
    {
        m_stack.top().item->insert(m_identifier, node {});
        m_stack.emplace(&m_stack.top().item->at(m_identifier), 0);
    }

    void state::close_block()
    {
        m_stack.pop();
    }

    void state::push(const node data)
    {
        node& top = *m_stack.top().item;
        if (top.is<array>())
        {
            top.push(data);
        }
        else if (top.is<object>())
        {
            top.insert(m_identifier, data);
        }
        else
        {
            throw std::runtime_error("Should not happen");
        }
        if (data.is_container())
        {
            m_stack.emplace(&top.back(), 0);
        }
    }

    void state::push_template()
    {
        node& top = *m_stack.top().item;
        if (top.size() == 0)
            m_templates[m_identifier] = top;
        else
            m_templates[m_identifier] = top.back();
    }

    node state::get_template(const std::string& template_name) const
    {
        return m_templates.at(template_name);
    }

    template <typename Rule> struct action
    {
    };

    template <> struct action<rules::string_content>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(in.string());
        }
    };

    template <> struct action<rules::number>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(std::stod(in.string()));
        }
    };

    template <> struct action<rules::integer>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(std::stoll(in.string()));
        }
    };

    template <> struct action<rules::boolean>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push((in.string() == "true" ? true : false));
        }
    };

    template <> struct action<rules::template_usage>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(state.get_template(in.string()));
        }
    };

    template <> struct action<rules::identifier>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.set_active_identifier(in.string());
        }
    };

    template <> struct action<rules::open_array>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(vili::array {});
        }
    };

    template <> struct action<rules::close_array>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.close_block();
        }
    };

    template <> struct action<rules::open_object>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(vili::object {});
        }
    };

    template <> struct action<rules::close_object>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.close_block();
        }
    };

    template <> struct action<rules::indent_based_object>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push(vili::object {});
            state.use_indent();
        }
    };

    template <> struct action<rules::indent>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.set_indent(in.string().size());
        }
    };

    template <> struct action<rules::template_keyword>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.set_indent(0);
        }
    };

    template <> struct action<rules::template_decl>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            state.push_template();
        }
    };

    vili::node from_string(std::string_view data)
    {
        state parser_state;
        peg::string_input in(data, "string_source");
        std::cout << "Parsing : " << data << std::endl;

        try
        {
            peg::parse<vili::parser::rules::grammar, vili::parser::action>(
                in, parser_state);
            /*peg::standard_trace<vili::parser::rules::grammar, vili::parser::action>(
                in, parser_state);*/
        }
        catch (peg::parse_error& e)
        {
            const auto p = e.positions.front();
            std::cerr << e.what() << '\n'
                      << in.line_at(p) << '\n'
                      << std::setw(p.byte_in_line) << ' ' << '^' << std::endl;
        }
        /*catch (vili::exceptions::base_exception& e)
        {
            std::cerr << "vili::exception : " << e.what() << std::endl;
        }*/

        return parser_state.root;
    }

    vili::node from_file(std::string_view path)
    {
        // TODO: File not found exception
        std::ifstream input_file;
        input_file.open(path.data());
        std::stringstream buffer;
        buffer << input_file.rdbuf();
        const std::string content = buffer.str();
        return from_string(content);
    }
}