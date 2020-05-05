#pragma once

#include <string>
#include <variant>

#include <vili/exceptions.hpp>
#include <vili/types.hpp>

namespace vili
{
    /**
     * \brief Base Class for every Node in the Tree
     */
    class node
    {
    protected:
        std::variant<object, array, integer, number, boolean, string> m_data;
        [[nodiscard]] std::string dump_array() const;
        [[nodiscard]] std::string dump_object(bool root) const;

    public:
        node();
        node(int value);
        node(integer value);
        node(number value);
        node(const string& value);
        node(boolean value);
        node(const char* value);
        node(const array& value);
        node(const object& value);
        node(const node& copy);
        [[nodiscard]] node_type type() const;
        [[nodiscard]] std::string dump(bool root = false) const;
        template <node_type type> bool is();
        template <class T>[[nodiscard]] bool is() const;
        [[nodiscard]] bool is_primitive() const;
        [[nodiscard]] bool is_null() const;
        template <class T> T& as();
        template <class T> const T& as() const;
        node& operator[](const std::string& key);
        node& operator[](size_t index);
        [[nodiscard]] size_t size() const;
        void push(const node& value);
        void insert(size_t index, const node& value);
        void insert(const std::string& key, node value);
    };

    template <node_type type> bool node::is()
    {
        return is<decltype(node_helper_t<type>::type)>();
    }

    template <class T> bool node::is() const
    {
        return std::holds_alternative<T>(m_data);
    }

    template <class T> const T& node::as() const
    {
        if (is<T>())
            return std::get<T>(m_data);
        throw exceptions::invalid_cast(typeid(T).name(), to_string(type()), EXC_INFO);
    }

    template <class T> T& node::as()
    {
        if (is<T>())
            return std::get<T>(m_data);
        throw exceptions::invalid_cast(typeid(T).name(), to_string(type()), EXC_INFO);
    }
}
