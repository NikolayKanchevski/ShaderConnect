//
// Created by Nikolay Kanchevski on 1.6.2024.
//

#pragma once

#include "../../ShaderCompiler.h"
#include "../../ShaderType.h"

namespace ShaderConnect
{

    class DXILShaderCompiler final : public ShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        inline explicit DXILShaderCompiler(const ShaderType shaderType) : shaderType(shaderType) { }

        /* --- POLLING METHODS --- */
        std::filesystem::path CompileShader(const std::filesystem::path &inputHLSLShaderFilePath, const std::filesystem::path &outputShaderDirectory);

        /* --- DESTRUCTOR --- */
        ~DXILShaderCompiler() = default;

    private:
        ShaderType shaderType = ShaderType::Vertex;

    };

}
