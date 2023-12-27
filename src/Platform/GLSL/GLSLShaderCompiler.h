//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include "../../ShaderCompiler.h"

namespace ShaderConnect
{

    class GLSLShaderCompiler final : public ShaderCompiler
    {
    public:
        /* --- POLLING METHODS --- */
        void CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderFilePath) override;

    };

}
