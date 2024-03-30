//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

#include <shaderc/shaderc.hpp>

#include "File.h"
#include "ShaderType.h"
#include "InputShaderLanguage.h"
#include "OutputShaderLanguage.h"

namespace ShaderConnect
{

    class CrossShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        CrossShaderCompiler(InputShaderLanguage inputShaderLanguage, ShaderType inputShaderType, const std::filesystem::path &inputShaderFilePath);

        /* --- POLLING METHODS --- */
        std::filesystem::path Compile(OutputShaderLanguage outputShaderLanguage, const std::filesystem::path &outputShaderDirectory);

        /* --- DESTRUCTOR --- */
        ~CrossShaderCompiler() = default;

    private:
        const InputShaderLanguage inputShaderLanguage;
        const ShaderType inputShaderType = ShaderType::Undefined;
        const std::filesystem::path inputShaderFilePath;

        class Includer : public shaderc::CompileOptions::IncluderInterface
        {
        public:
            /* --- CONSTRUCTORS --- */
            explicit Includer(std::filesystem::path filePath);

            /* --- GETTER METHODS --- */
            [[nodiscard]] shaderc_include_result* GetInclude(const char* requestedSource, shaderc_include_type type, const char* requestingSource, size_t includeDepth) override;

            /* --- DESTRUCTOR --- */
            void ReleaseInclude(shaderc_include_result* data) override;

        private:
            std::filesystem::path filePath;

        };

    };

}
