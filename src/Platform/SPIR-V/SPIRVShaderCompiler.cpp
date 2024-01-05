//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "SPIRVShaderCompiler.h"

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    std::filesystem::path SPIRVShaderCompiler::CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderDirectory)
    {
        const std::filesystem::path outputShaderFilePath = outputShaderDirectory / "shader.spv";
        File::WriteToFile(outputShaderFilePath, reinterpret_cast<const char*>(spirvBuffer.data()), spirvBuffer.size() * sizeof(uint32), true, true);
        return outputShaderFilePath;
    }

}