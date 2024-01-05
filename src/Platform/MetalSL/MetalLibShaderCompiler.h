//
// Created by Nikolay Kanchevski on 4.01.24.
//

#pragma once

#include "../../ShaderCompiler.h"

namespace ShaderConnect
{

    enum class MetalLibTargetPlatform : uint8
    {
        macOS,
        iOS,
        iOSSimulator
    };

    class MetalLibShaderCompiler : public ShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        inline explicit MetalLibShaderCompiler(const MetalLibTargetPlatform targetPlatform) : targetPlatform(targetPlatform) { };

        /* --- POLLING METHODS --- */
        std::filesystem::path CompileShader(const std::filesystem::path &inputMetalShaderFilePath, const std::filesystem::path &outputShaderDirectory);

        /* --- DESTRUCTOR --- */
        ~MetalLibShaderCompiler() = default;

    private:
        MetalLibTargetPlatform targetPlatform;

    };

}
