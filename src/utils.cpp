#include <vili/utils.hpp>

#include <algorithm>
#include <cctype>

namespace vili::utils::string
{
    constexpr auto is_digit = static_cast<int (*)(int)>(std::isdigit);
    size_t count(const std::string& str, const std::string& occur)
    {
        int occurrences = 0;
        std::string::size_type start = 0;
        while ((start = str.find(occur, start)) != std::string::npos)
        {
            ++occurrences;
            start += occur.length();
        }
        return occurrences;
    }

    bool is_int(const std::string& str)
    {
        if (!str.empty())
        {
            if (str.substr(0, 1) == "-")
                return std::all_of(str.begin() + 1, str.end(), is_digit);
            return std::all_of(str.begin(), str.end(), is_digit);
        }
        return false;
    }

    bool is_float(const std::string& str)
    {
        std::string float_str = str;
        bool is_a_float = false;
        if (!float_str.empty())
        {
            if (float_str.substr(0, 1) == "-")
                float_str = float_str.substr(1);
            if (count(float_str, ".") == 1)
            {
                is_a_float = true;
                float_str.erase(float_str.find('.'));
            }
            return (
                std::all_of(float_str.begin(), float_str.end(), is_digit) && is_a_float);
        }
        return false;
    }

    std::string truncate_float(const std::string& str)
    {
        std::string result = str;
        while (
            result.size() > 2 && result.back() == '0' && result[result.size() - 2] != '.')
            result.pop_back();
        return result;
    }

    std::string quote(const std::string& str)
    {
        return "\"" + str + "\"";
    }

    std::string replace(
        std::string subject, const std::string& search, const std::string& replace)
    {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos)
        {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }
}