#pragma once

#include <cstring>
#include <iostream>
#include <assert.h>
#include <source_location>

namespace cpp_low_latency
{
    namespace common
    {
        using namespace std;
#define ASSERT(value, msg) CHECK(value, msg);

        // forward declaration to avoid multiple definition.
        inline void CHECK(bool check,
                          const std::string &msg,
                          std::source_location location = std::source_location::current())
        {
            if (!check)
            {
                cout << format("ASSERT() failed at {} {} {}, error: {}", location.file_name(), location.line(), location.function_name(), msg);
                assert(false);
                exit(EXIT_FAILURE);
            }
        }
    }
}