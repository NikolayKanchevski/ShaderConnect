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

        /* --- POLLING METHODS --- */
        virtual void CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderFilePath) = 0;

        /* --- DESTRUCTOR --- */
        virtual ~ShaderCompiler() = default;
    };

}
