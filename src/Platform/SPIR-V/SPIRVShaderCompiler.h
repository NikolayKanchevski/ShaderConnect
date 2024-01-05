//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include "../../ShaderCompiler.h"

namespace ShaderConnect
{

    class SPIRVShaderCompiler final : public ShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        SPIRVShaderCompiler() = default;

        /* --- POLLING METHODS --- */
        std::filesystem::path CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderDirectory);

        /* --- DESTRUCTOR --- */
        ~SPIRVShaderCompiler() = default;

    };

}
