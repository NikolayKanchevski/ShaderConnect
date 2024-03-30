//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include <cstdint>

#if defined(__cplusplus)
    #pragma region Standard Library
        #include <cstdio>
        #include <array>
        #include <vector>
        #include <string>
        #include <string_view>
        #include <fstream>
        #include <exception>
        #include <filesystem>
        #include <memory>
    #pragma endregion

    #pragma region Platform Detection
        #define SC_PLATFORM_WINDOWS 0
        #define SC_PLATFORM_macOS 0
        #if _WIN32 || _WIN64
            #undef SC_PLATFORM_WINDOWS
            #define SC_PLATFORM_WINDOWS 1
        #elif __APPLE__ || __MACH__
            #undef SC_PLATFORM_macOS
            #define SC_PLATFORM_macOS 1
        #endif
    #pragma endregion

    #pragma region Type Definitions
        typedef int8_t int8;
        typedef int16_t int16;
        typedef int32_t int32;
        typedef int64_t int64;
        typedef uint8_t uint8;
        typedef uint16_t uint16;
        typedef uint32_t uint32;
        typedef uint64_t uint64;
        typedef float float32;
        typedef double float64;
    #pragma endregion
#endif