//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "SPIRVShaderCompiler.h"

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    void SPIRVShaderCompiler::CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderFilePath)
    {
        File::WriteToFile(outputShaderFilePath.string() + ".spv", reinterpret_cast<const char*>(spirvBuffer.data()), spirvBuffer.size() * sizeof(uint32), true, true);
    }

}