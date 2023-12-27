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
        macOSMSL,
        iOSMSL,
        SPIRV
    };

    static OutputShaderLanguage StringToOutputShaderLanguageLanguage(const char* string)
    {
        if (strcmp(string, "glsl") == 0)           return OutputShaderLanguage::GLSL;
        if (strcmp(string, "essl") == 0)           return OutputShaderLanguage::ESSL;
        if (strcmp(string, "hlsl") == 0)           return OutputShaderLanguage::HLSL;
        if (strcmp(string, "macos-msl") == 0)      return OutputShaderLanguage::macOSMSL;
        if (strcmp(string, "ios-msl") == 0)        return OutputShaderLanguage::iOSMSL;
        if (strcmp(string, "spir-v") == 0)         return OutputShaderLanguage::SPIRV;
        return OutputShaderLanguage::Undefined;
    }

}