//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include "OutputShaderLanguage.h"

#include "File.h"

namespace ShaderConnect
{

    class ShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        ShaderCompiler() = default;

        /* --- OPERATORS --- */
        ShaderCompiler(const ShaderCompiler&) = delete;
        ShaderCompiler &operator=(const ShaderCompiler&) = delete;

        /* --- DESTRUCTOR --- */
        virtual ~ShaderCompiler() = default;
    };

}
