#include <iterator>
#include <vili/node.hpp>

namespace vili
{
    std::string node::dump_array() const
    {
        const auto& vector = std::get<array>(m_data);
        std::string dump_value = "[";
        for (auto it = vector.begin(); it != vector.end(); ++it)
        {
            dump_value += it->dump() + (it != (vector.end() - 1) ? ", " : "");
        }
        dump_value += "]";
        return dump_value;
    }

    std::string node::dump_object(bool root) const
    {
        const auto& map = std::get<object>(m_data);
        std::string dump_value = (root) ? "" : "{";
        for (auto it = map.second.begin(); it != map.second.end(); ++it)
        {
            dump_value += *it + ": " + map.first.at(*it).dump()
                + (it != (map.second.end() - 1) ? ", " : "");
        }
        if (!root)
            dump_value += "}";
        return dump_value;
    }

    node::node()
    {
        // std::cout << "- Create new null node : " << m_data.index() << std::endl;
    }

    node::node(int value)
    {
        m_data = static_cast<int64_t>(value);
        // std::cout << "- Create new integer node : " << m_data.index() << std::endl;
    }

    node::node(integer value)
    {
        m_data = value;
    }

    node::node(number value)
    {
        m_data = value;
    }

    node::node(const string& value)
    {
        m_data = value;
    }

    node::node(boolean value)
    {
        m_data = value;
    }

    node::node(const char* value)
    {
        m_data = std::string(value);
    }

    node::node(const array& value)
    {
        m_data = value;
    }

    node::node(const object& value)
    {
        m_data = value;
    }

    node::node(const node& copy)
    {
        m_data = copy.m_data;
    }

    node_type node::type() const
    {
        if (is_null())
            return node_type::null;
        if (std::holds_alternative<integer>(m_data))
            return node_type::integer;
        if (std::holds_alternative<number>(m_data))
            return node_type::number;
        if (std::holds_alternative<boolean>(m_data))
            return node_type::boolean;
        if (std::holds_alternative<string>(m_data))
            return node_type::string;
        if (std::holds_alternative<object>(m_data))
            return node_type::object;
        if (std::holds_alternative<array>(m_data))
            return node_type::array;
    }

    std::string node::dump(bool root) const
    {
        if (is_null())
            return "";
        if (is<number>())
            return utils::string::truncate_float(std::to_string(as<number>()));
        if (is<integer>())
            return std::to_string(as<integer>());
        if (is<string>())
            return utils::string::quote(as<string>());
        if (is<boolean>())
            return (as<boolean>() ? "true" : "false");
        if (is<array>())
            return dump_array();
        if (is<object>())
            return dump_object(root);
    }

    bool node::is_primitive() const
    {
        if (is<integer>() || is<number>() || is<string>() || is<boolean>())
            return true;
        return false;
    }

    bool node::is_null() const
    {
        if (is<object>() || is<array>() || is<integer>() || is<string>() || is<number>()
            || is<boolean>())
            return false;
        return true;
    }

    node& node::operator[](const std::string& key)
    {
        if (is<object>())
        {
            auto& map = std::get<object>(m_data);
            if (const auto& item = map.first.find(key); item != map.first.end())
            {
                return item->second;
            }
            throw exceptions::unknown_child_node(key, EXC_INFO);
        }
        throw exceptions::invalid_cast(object_type, to_string(type()), EXC_INFO);
    }

    node& node::operator[](const size_t index)
    {
        if (is<array>())
        {
            auto& vector = std::get<array>(m_data);
            if (index < vector.size())
            {
                return vector.at(index);
            }
            throw exceptions::array_index_overflow(index, vector.size(), EXC_INFO);
        }
        throw exceptions::invalid_cast(array_type, to_string(type()), EXC_INFO);
    }

    size_t node::size() const
    {
        if (is<array>())
        {
            return std::get<array>(m_data).size();
        }
        if (is<object>())
        {
            return std::get<object>(m_data).first.size();
        }
        throw exceptions::invalid_cast(object_type, to_string(type()), EXC_INFO);
    }

    void node::push(const node& value)
    {
        if (is<array>())
        {
            std::get<array>(m_data).push_back(value);
        }
        else
        {
            throw exceptions::invalid_cast(array_type, to_string(type()), EXC_INFO);
        }
    }

    void node::insert(size_t index, const node& value)
    {
        if (is<array>())
        {
            auto& vector = std::get<array>(m_data);
            vector.insert(vector.cbegin() + index, value);
        }
        else
        {
            throw exceptions::invalid_cast(array_type, to_string(type()), EXC_INFO);
        }
    }

    void node::insert(const std::string& key, node value)
    {
        if (is<object>())
        {
            auto& map = std::get<object>(m_data);
            map.first.emplace(key, std::move(value));
            map.second.push_back(key);
        }
        else
        {
            throw exceptions::invalid_cast(array_type, to_string(type()), EXC_INFO);
        }
    }
}
