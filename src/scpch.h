//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include <cstdint>

#if defined(__cplusplus)
    #pragma region Standard Library
        #include <cstdio>
        #include <vector>
        #include <cstring>
        #include <fstream>
        #include <exception>
        #include <filesystem>
        #if defined _MSC_VER
            #include <direct.h>
        #elif defined __GNUC__
            #include <sys/stat.h>
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