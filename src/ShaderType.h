//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

namespace ShaderConnect
{

    enum class ShaderType
    {
        Undefined,
        Vertex,
        Fragment,
        Compute
    };

    static ShaderType StringToShaderType(const char* string)
    {
        if (strcmp(string, "vertex") == 0)          return ShaderType::Vertex;
        if (strcmp(string, "fragment") == 0)        return ShaderType::Fragment;
        if (strcmp(string, "compute") == 0)         return ShaderType::Compute;
        return ShaderType::Undefined;
    }

}