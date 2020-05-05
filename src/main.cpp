#include <vili/nodes/node.hpp>
#include <vili/types.hpp>

// #include <picobench/picobench.hpp>

#include <fstream>
#include <iostream>
#include <stack>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

namespace peg = tao::pegtl;
using namespace std::string_literals;

namespace vili::parser
{
    namespace rules
    {
        // clang-format off
        struct identifier : peg::identifier {};
        struct indent : peg::seq<peg::bol, peg::star<peg::blank>> {};
        struct affectation : peg::must<identifier, peg::pad<peg::one<':'>, peg::blank>> {};

        // String
        struct string : peg::seq<peg::one<'"'>, peg::star<peg::not_one<'"'>>, peg::one<'"'>> {};

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

        // Arrays
        struct array_elements : peg::list<data, peg::one<','>, peg::blank> {};
        struct open_array : peg::one<'['> {};
        struct close_array : peg::one<']'> {};
        struct array : peg::seq<open_array, peg::pad_opt<array_elements, peg::space>, close_array> {};

        // Objects
        struct node;
        struct indent_based_object : peg::eol {};
        struct open_object : peg::one<'{'> {};
        struct close_object : peg::one<'}'> {};
        struct comma_or_newline : peg::seq<peg::pad<peg::sor<peg::one<','>, peg::eol>, peg::space>, peg::star<peg::space>> {};
        struct object_elements : peg::list<node, comma_or_newline> {};
        struct brace_based_object : peg::seq<open_object, peg::pad_opt<object_elements, peg::space>, close_object> {};
        struct object : peg::sor<brace_based_object, indent_based_object> {};

        // Nodes
        // struct data_node : peg::seq<indent, affectation, data> {};
        // struct array_node : peg::seq<indent, affectation, array> {};
        // struct object_node : peg::seq<indent, affectation> {};
        struct node : peg::seq<affectation, peg::sor<data, array, object>> {};
        struct line : peg::seq<indent, node, peg::opt<peg::eol>> {};
        struct grammar : peg::until<peg::eof, peg::must<line>> {};
        // clang-format on
    }

    class state
    {
    private:
        std::string m_identifier;
        std::stack<std::pair<node*, int>> m_stack;
        int64_t m_indent_base = 4;
        int64_t m_indent_current = -1;

    public:
        node root;
        state();
        void set_indent(int64_t indent);
        void use_indent();
        void set_active_identifier(const std::string& identifier);
        void open_block();
        void close_block();
        template <class vili_type> void push(vili_type data);
    };

    state::state()
        : root(object {})
    {
        m_stack.push(std::make_pair(&root, 0));
    }

    void state::set_indent(int64_t indent)
    {
        if (m_indent_current == -1 && indent > 0)
        {
            m_indent_base = indent;
        }
        if (indent % m_indent_base && m_stack.top().second)
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
        else if (m_indent_current == indent && indent < m_stack.top().second)
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
        // std::cout << "================ INDENT FROM " << m_indent_current << " TO " << indent << " / STACK : " << m_stack.size() << std::endl;
        m_indent_current = indent;
    }

    void state::use_indent()
    {
        m_stack.top().second = (m_indent_current + 1);
    }

    void state::set_active_identifier(const std::string& identifier)
    {
        m_identifier = identifier;
    }

    void state::open_block()
    {
        // std::cout << "Opening block with identifier " << m_identifier << std::endl;
        m_stack.top().first->insert(m_identifier, node {});
        m_stack.push(std::make_pair(&m_stack.top().first->operator[](m_identifier), 0));
        // std::cout << "======== STACK OPEN : " << m_stack.size() << std::endl;
    }

    void state::close_block()
    {
        m_stack.pop();
        // std::cout << "======== STACK EXIT : " << m_stack.size() << std::endl;
    }

    template <class vili_type> void state::push(vili_type data)
    {
        if (m_stack.top().first->type() == vili::node_type::array)
        {
            m_stack.top().first->push(data);
        }
        else
        {
            *m_stack.top().first = data;
            if (m_stack.top().first->is_primitive())
                m_stack.pop();
            // std::cout << "stack size : " << m_stack.size() << std::endl;
        }
    }

    template <typename Rule> struct action
    {
    };

    template <> struct action<rules::string>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Found quote : " << in.string() << std::endl;
            auto content = in.string();
            state.push(content.substr(1, content.size() - 2));
        }
    };

    template <> struct action<rules::number>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Found float : " << in.string() << std::endl;
            state.push(std::stod(in.string()));
        }
    };

    template <> struct action<rules::integer>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Found integer : " << in.string() << std::endl;
            state.push(std::stoll(in.string()));
        }
    };

    template <> struct action<rules::boolean>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Found boolean : " << in.string() << std::endl;
            state.push((in.string() == "true" ? true : false));
        }
    };

    template <> struct action<rules::affectation>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Found affectation : " << in.string() << std::endl;
            state.open_block();
        }
    };

    template <> struct action<rules::identifier>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Found identifier : " << in.string() << std::endl;
            state.set_active_identifier(in.string());
        }
    };

    template <> struct action<rules::open_array>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Opening array : " << in.string() << std::endl;
            state.push(vili::array {});
        }
    };

    template <> struct action<rules::close_array>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Closing array : " << in.string() << std::endl;
            state.close_block();
        }
    };

    template <> struct action<rules::open_object>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Opening object : " << in.string() << std::endl;
            state.push(vili::object {});
        }
    };

    template <> struct action<rules::close_object>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Closing object : " << in.string() << std::endl;
            state.close_block();
        }
    };

    template <> struct action<rules::indent_based_object>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Opening [via indent] object : " << in.string() << std::endl;
            state.push(vili::object {});
            state.use_indent();
        }
    };

    template <> struct action<rules::indent>
    {
        template <class ParseInput> static void apply(const ParseInput& in, state& state)
        {
            // std::cout << "Set indent level to " << in.string().size() << std::endl;
            state.set_indent(in.string().size());
        }
    };
}

class Theme
{
private:
    std::string_view m_background;
    std::string_view m_foreground;
    std::string_view m_text;

public:
    void background(std::string_view bg)
    {
        m_background = bg;
    }
    void foreground(std::string_view fg)
    {
        m_foreground = fg;
    }
    void text(std::string_view tx)
    {
        m_text = tx;
    }
};

class Company
{
private:
    unsigned int m_budget;
    std::string_view m_name;
    std::string_view m_description;

public:
    void budget(unsigned int bg)
    {
        m_budget = bg;
    }
    void name(std::string_view nm)
    {
        m_name = nm;
    }
    void description(std::string_view dc)
    {
        m_description = dc;
    }
};

class Citizen
{
private:
    std::string_view m_name;
    std::string_view m_address;
    std::string_view m_iban;
    Theme m_theme;
    std::vector<Company> m_companies;
    std::string_view m_expire;

public:
    void name(std::string_view nm)
    {
        m_name = nm;
    }
    void address(std::string_view ad)
    {
        m_address = ad;
    }
    void iban(std::string_view ib)
    {
        m_iban = ib;
    }
    void theme(const Theme& tm)
    {
        m_theme = tm;
    }
    void company(const Company& cp)
    {
        m_companies.push_back(cp);
    }
    void expire(std::string_view xp)
    {
        m_expire = xp;
    }
};

/*void vili_parse_pico(picobench::state& s)
{
    std::ifstream input_file;
    input_file.open("data.vili");
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    // std::cout << buffer.str() << std::endl;
    vili::parser::state state;
    // peg::file_input in("data.vili");
    std::string content = buffer.str();
    peg::string_input in(content, "string_source");
    s.start_timer();
    peg::parse<vili::parser::rules::grammar, vili::parser::action>(in, state);
    std::vector<Citizen> citizens;
    vili::node& data = state.root["data"];
    citizens.reserve(data.size());
    for (auto& lcitizen : data.as<vili::object>().first)
    {
        Citizen citizen;
        citizen.name(lcitizen.first);
        vili::node& lcitizen_content = lcitizen.second;
        citizen.address(lcitizen.second["address"].as<vili::string>());
        citizen.iban(lcitizen_content["iban"].as<vili::string>());
        citizen.expire(lcitizen_content["expires"].as<vili::string>());
        vili::object& companies = lcitizen_content["companies"].as<vili::object>();
        for (auto& lcompany : companies.first)
        {
            Company company;
            company.name(lcompany.first);
            vili::node& lcompany_content = lcompany.second;
            company.budget(lcompany_content["budget"].as<vili::integer>());
            company.description(lcompany_content["description"].as<vili::string>());
            citizen.company(company);
        }
        Theme theme;
        theme.background(lcitizen_content["theme"]["background"].as<vili::string>());
        theme.foreground(lcitizen_content["theme"]["foreground"].as<vili::string>());
        theme.text(lcitizen_content["theme"]["text"].as<vili::string>());
        citizen.theme(theme);
        citizens.push_back(std::move(citizen));
    }
    s.stop_timer();
}*/

// PICOBENCH(vili_parse_pico).iterations({ 1 });

void vili_parse()
{
    std::ifstream input_file;
    input_file.open("data.vili");
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    // std::cout << buffer.str() << std::endl;
    vili::parser::state state;
    // peg::file_input in("data.vili");
    std::string content = buffer.str();
    peg::string_input in(content, "string_source");
    peg::parse<vili::parser::rules::grammar, vili::parser::action>(in, state);
    std::vector<Citizen> citizens;
    vili::node& data = state.root["data"];
    citizens.reserve(data.size());
    for (auto& lcitizen : data.as<vili::object>().first)
    {
        Citizen citizen;
        citizen.name(lcitizen.first);
        vili::node& lcitizen_content = lcitizen.second;
        citizen.address(lcitizen.second["address"].as<vili::string>());
        citizen.iban(lcitizen_content["iban"].as<vili::string>());
        citizen.expire(lcitizen_content["expires"].as<vili::string>());
        vili::object& companies = lcitizen_content["companies"].as<vili::object>();
        for (auto& lcompany : companies.first)
        {
            Company company;
            company.name(lcompany.first);
            vili::node& lcompany_content = lcompany.second;
            company.budget(lcompany_content["budget"].as<vili::integer>());
            company.description(lcompany_content["description"].as<vili::string>());
            citizen.company(company);
        }
        Theme theme;
        theme.background(lcitizen_content["theme"]["background"].as<vili::string>());
        theme.foreground(lcitizen_content["theme"]["foreground"].as<vili::string>());
        theme.text(lcitizen_content["theme"]["text"].as<vili::string>());
        citizen.theme(theme);
        citizens.push_back(std::move(citizen));
    }
}

int main()
{
    vili_parse();
    return 0;
}

/*int main(int argc, char** argv)
{
    std::ifstream input_file;
    input_file.open("data.vili");
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    // std::cout << buffer.str() << std::endl;
    vili::parser::state state;
    // peg::file_input in("data.vili");
    std::string content = buffer.str();
    peg::string_input in(content, "string_source");

    std::ofstream err_output("debug.log");
    std::ios_base::sync_with_stdio(false);
    std::cerr.tie(0);

    // std::cerr.rdbuf(err_output.rdbuf());

    try
    {
        // peg::parse<vili::parser::rules::grammar, vili::parser::action>(in, state);
        // std::cout << "DUMP :\n" << state.root.dump(true) << std::endl;
        // std::cout << "Size : " << state.root["data"].size() << std::endl;
        std::cout << "================" << std::endl;
    }
    catch (peg::parse_error& e)
    {
        const auto p = e.positions.front();
        std::cerr << e.what() << '\n'
                  << in.line_at(p) << '\n'
                  << std::setw(p.byte_in_line) << ' ' << '^' << std::endl;
    }
    return 0;
}*/