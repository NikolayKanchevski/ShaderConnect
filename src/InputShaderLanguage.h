//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

namespace ShaderConnect
{

    enum class InputShaderLanguage
    {
        Undefined,
        GLSL,
        HLSL
    };

    static InputShaderLanguage StringToInputShaderLanguage(const char* string)
    {
        if (strcmp(string, "glsl") == 0)        return InputShaderLanguage::GLSL;
        if (strcmp(string, "hlsl") == 0)        return InputShaderLanguage::HLSL;
        return InputShaderLanguage::Undefined;
    }

}