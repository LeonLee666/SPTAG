// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace SPTAG { namespace SSDServing { namespace SSDIndex {
// Keep line positions intact: blank lines and comments are errors, not skipped records.
inline bool LoadQueryInternalResultNums(const std::string& path, std::vector<int>& values, std::string& error)
{
    values.clear();
    error.clear();
    std::ifstream input(path);
    if (!input.is_open()) {
        error = "Cannot open InternalResultNumFile: " + path;
        return false;
    }
    std::string line;
    size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        const auto first = line.find_first_not_of(" \t\r\n\v\f");
        const auto last = line.find_last_not_of(" \t\r\n\v\f");
        int value = 0;
        bool valid = first != std::string::npos;
        if (valid) {
            for (size_t i = first; i <= last; ++i) {
                const char c = line[i];
                if (c < '0' || c > '9' || value > ((std::numeric_limits<int>::max)() - (c - '0')) / 10) {
                    valid = false;
                    break;
                }
                value = value * 10 + (c - '0');
            }
        }
        if (!valid || value <= 0) {
            error = "Invalid positive integer in InternalResultNumFile: " + path + ", line " + std::to_string(lineNumber);
            values.clear();
            return false;
        }
        values.push_back(value);
    }
    if (input.bad()) {
        error = "Failed reading InternalResultNumFile: " + path;
        values.clear();
        return false;
    }
    return true;
}
}}}
