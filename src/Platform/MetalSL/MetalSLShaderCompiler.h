//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include "../../ShaderCompiler.h"

namespace ShaderConnect
{

    enum class MetalSLTargetPlatform : bool
    {
        macOS,
        iOS
    };

    class MetalSLShaderCompiler final : public ShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        inline explicit MetalSLShaderCompiler(const MetalSLTargetPlatform targetPlatform) : targetPlatform(targetPlatform) { }

        /* --- POLLING METHODS --- */
        std::filesystem::path CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderDirectory);

        /* --- DESTRUCTOR --- */
        ~MetalSLShaderCompiler() = default;

    private:
        MetalSLTargetPlatform targetPlatform = MetalSLTargetPlatform::macOS;

    };

}
