/*
   Copyright 2026 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once

#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <string>
#include <ctype.h>
#include <commons/allocators/resource.hh>


namespace cm {

///
/// Creates a JSON representation for a given instance of a struct
///
inline auto jsonify(auto const& obj, Arena<char>& allocator) -> MutableStringRef
{
    using T = std::remove_cvref_t<decltype(obj)>;
    static_assert(__is_class(T), "Only instances of a struct can be converted to JSON objects");

    static thread_local Arena<char>* s_allocator{};
    static thread_local char s_dummy[1]{};
    static thread_local MutableStringRef s_buf = {s_dummy, 0};

    s_allocator = &allocator;


    constexpr static auto tabstr = [](unsigned int n) {
        switch (n) {
        case 0: return "";
        case 1: return "  ";
        case 2: return "    ";
        case 3: return "      ";
        case 4: return "        ";
        case 5: return "          ";
        case 6: return "            ";
        case 7: return "              ";
        case 8: return "                ";
        case 9: return "                  ";
        case 10: return "                    ";
        case 11: return "                      ";
        case 12: return "                        ";
        case 13: return "                          ";
        case 14: return "                            ";
        case 15: return "                              ";
        default: return "                                ";
        }
    };

    constexpr static auto transformToJson = [](std::string const& input) -> std::string {
        std::stringstream ss(input);
        std::string token;
        std::string json = "";
        unsigned int tabs = 0;

        bool first_item = true;

        // Skip the root name (e.g., "S")
        std::string prev_token = token;
        ss >> token;

        while (ss >> token) {
            // printf("%s -> %s\n", prev_token.c_str(), token.c_str());

            if (token == "{") {
                json += "{\n";
                tabs++;
                first_item = true;
            } else if (token == "}") {
                json += "\n";
                tabs--;
                json += tabstr(tabs);
                json += "}";
            } else if (token == "=") {
                if (!first_item) {
                    json += ",\n";
                }
                json += tabstr(tabs);
                json += "\"" + prev_token + "\": ";
                first_item = false;
            } else if (prev_token == "=" && !token.empty()) {
                if (isdigit(token[0]) || token[0] == '-') {
                    json += token;
                } else if (token[0] != '"' && token[token.size() - 1] != '"') {
                    json += "\"" + token + "\"";
                } else {
                    json += token;
                }
            }
            prev_token = token;
        }
        return json;
    };

    //__builtin_dump_struct(&obj, printf);
    // puts("");
    __builtin_dump_struct(&obj, [](char const* fmt, ...) -> int {
        va_list args1, args2;
        va_start(args1, fmt);
        va_copy(args2, args1);

        int size = vsnprintf(nullptr, 0, fmt, args1);
        va_end(args1);

        if (size > 0) {
            size_t old_size = buf.size();
            buf.resize(old_size + size);
            vsnprintf(&buf[old_size], size + 1, fmt, args2);
        }
        va_end(args2);
        return 0;
    });


    s_allocator = nullptr;
    return result;
}

}  // namespace cm
