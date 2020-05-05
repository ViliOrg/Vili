#pragma once

#include <exception>
#include <optional>
#include <string>

#include <fmt/format.h>

#include <vili/utils.hpp>

namespace vili::exceptions
{
    class debug_info
    {
    public:
        std::string_view file;
        int line;
        std::string_view function;
        debug_info(std::string_view file, int line, std::string_view function)
            : file(file)
            , line(line)
            , function(function)
        {
        }
    };
#define EXC_INFO_WRAPPER() exceptions::debug_info(__FILE__, __LINE__, __FUNCTION__)
#define EXC_INFO EXC_INFO_WRAPPER()
    class base_exception : public std::exception
    {
    private:
        std::string m_message;

    public:
        base_exception(std::string id, debug_info info)
        {
            m_message = fmt::format("exception [{}] occured\n", id);
            m_message += fmt::format("  In file: '{}' (line {})\n", info.file, info.line);
            m_message += fmt::format("  In function: {}\n", info.function);
        }
        template <class... Args> void error(Args&&... args)
        {
            const std::string errorMsg = fmt::format(std::forward<Args>(args)...);
            m_message += fmt::format("  Error: {}\n", errorMsg);
        }
        template <class... Args> void hint(Args&&... args)
        {
            const std::string hintMsg = fmt::format(std::forward<Args>(args)...);
            m_message += fmt::format("  Hint: {}\n", hintMsg);
        }
        const char* what() const noexcept override;
        base_exception& nest(const base_exception& exception)
        {
            m_message += "  Cause:\n";
            m_message
                += "    " + utils::string::replace(exception.what(), "\n", "\n    ");
            return *this;
        }
    };

    inline const char* base_exception::what() const noexcept
    {
        return m_message.c_str();
    }

    class invalid_cast : public base_exception
    {
    public:
        invalid_cast(std::string_view cast, std::string_view real_type, debug_info info)
            : base_exception("invalid_cast", info)
        {
            this->error("Tried to cast a value of type <{}> to <{}>", real_type, cast);
        }
    };

    class unknown_child_node : public base_exception
    {
    public:
        unknown_child_node(std::string_view key, debug_info info)
            : base_exception("unknown_child_node", info)
        {
            this->error("Tried to access non-existent child node '{}'", key);
        }
    };

    class array_index_overflow : public base_exception
    {
    public:
        array_index_overflow(size_t index, size_t maximum, debug_info info)
            : base_exception("array_index_overflow", info)
        {
            this->error(
                "Tried to access element {} of an array of size {}", index, maximum);
        }
    };

    class invalid_data_type : base_exception
    {
    public:
        invalid_data_type(debug_info info)
            : base_exception("invalid_data_type", info)
        {
            this->error("data_type is not a known value");
        }
    };

    class inconsistent_indentation : base_exception
    {
    public:
        inconsistent_indentation(
            int64_t indent_level, int64_t base_indentation, debug_info info)
            : base_exception("inconsistent_indentation", info)
        {
            this->error("File uses an inconsistent level of indentation ({}) where base "
                        "indentation is {}",
                indent_level, base_indentation);
        }
    };

    class too_much_indentation : base_exception
    {
    public:
        too_much_indentation(int64_t indent_level, debug_info info)
            : base_exception("too_much_indentation", info)
        {
            this->error(
                "Block with {} levels of indentation is too indented", indent_level);
        }
    };
}