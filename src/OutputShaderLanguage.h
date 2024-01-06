//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

namespace ShaderConnect
{

    enum class OutputShaderLanguage
    {
        Undefined,
        GLSL,
        ESSL,
        HLSL,
        DXIL,
        macOSMetalSL,
        iOSMetalSL,
        macOSMetalLib,
        iOSMetalLib,
        iOSSimulatorMetalLib,
        SPIRV
    };

    static OutputShaderLanguage StringToOutputShaderLanguageLanguage(const char* string)
    {
        if (strcmp(string, "glsl") == 0)                        return OutputShaderLanguage::GLSL;
        if (strcmp(string, "essl") == 0)                        return OutputShaderLanguage::ESSL;
        if (strcmp(string, "hlsl") == 0)                        return OutputShaderLanguage::HLSL;
        if (strcmp(string, "dxil") == 0)                        return OutputShaderLanguage::DXIL;
        if (strcmp(string, "macos-metalsl") == 0)               return OutputShaderLanguage::macOSMetalSL;
        if (strcmp(string, "ios-metalsl") == 0)                 return OutputShaderLanguage::iOSMetalSL;
        if (strcmp(string, "macos-metallib") == 0)              return OutputShaderLanguage::macOSMetalLib;
        if (strcmp(string, "ios-metallib") == 0)                return OutputShaderLanguage::iOSMetalLib;
        if (strcmp(string, "ios-simulator-metallib") == 0)      return OutputShaderLanguage::iOSSimulatorMetalLib;
        if (strcmp(string, "spir-v") == 0)                      return OutputShaderLanguage::SPIRV;
        return OutputShaderLanguage::Undefined;
    }

}