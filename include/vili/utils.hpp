#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace vili::utils::string
{
    std::string replace(
        std::string subject, const std::string& search, const std::string& replace);
    bool is_int(const std::string& str);
    bool is_float(const std::string& str);
    std::string truncate_float(const std::string& str);
    std::string quote(const std::string& str);
}