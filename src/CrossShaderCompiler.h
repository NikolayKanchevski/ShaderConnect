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

    struct ShaderCrossCompilerCreateInfo
    {
        const std::filesystem::path &inputShaderFilePath;
        InputShaderLanguage inputShaderLanguage = InputShaderLanguage::Undefined;
        ShaderType inputShaderType = ShaderType::Undefined;
    };

    class CrossShaderCompiler
    {
    public:
        /* --- CONSTRUCTORS --- */
        explicit CrossShaderCompiler(const ShaderCrossCompilerCreateInfo &createInfo);

        /* --- POLLING METHODS --- */
        void Compile(OutputShaderLanguage outputShaderLanguage, const std::filesystem::path &outputShaderDirectory) const;

        /* --- DESTRUCTOR --- */
        ~CrossShaderCompiler() = default;

    private:
        std::string inputShaderName;
        std::vector<uint32> inputShaderSpirvBuffer;
        ShaderType inputShaderType = ShaderType::Undefined;

        class Includer : public shaderc::CompileOptions::IncluderInterface
        {
        public:
            /* --- CONSTRUCTORS --- */
            explicit Includer(std::filesystem::path filePath);

            /* --- GETTER METHODS --- */
            [[nodiscard]] shaderc_include_result* GetInclude(const char* requestedSource, shaderc_include_type type, const char* requestingSource, unsigned long includeDepth) override;

            /* --- DESTRUCTOR --- */
            void ReleaseInclude(shaderc_include_result* data) override;

        private:
            std::filesystem::path filePath;

        };

    };

}
