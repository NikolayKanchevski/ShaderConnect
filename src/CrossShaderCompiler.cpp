//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "CrossShaderCompiler.h"

#include "Platform/GLSL/GLSLShaderCompiler.h"
#include "Platform/HLSL/HLSLShaderCompiler.h"
#include "Platform/HLSL/DXILShaderCompiler.h"
#include "Platform/MetalSL/MetalSLShaderCompiler.h"
#include "Platform/MetalSL/MetalLibShaderCompiler.h"
#include "Platform/SPIR-V/SPIRVShaderCompiler.h"

namespace ShaderConnect
{

    /* --- CONSTRUCTORS --- */

    CrossShaderCompiler::CrossShaderCompiler(const InputShaderLanguage inputShaderLanguage, const ShaderType inputShaderType, const std::filesystem::path &inputShaderFilePath)
        : inputShaderName(inputShaderFilePath.stem().string()), inputShaderType(inputShaderType)
    {
        if (inputShaderLanguage == InputShaderLanguage::Undefined) throw std::runtime_error("Input shader's language must not be InputShaderLanguage::Undefined!");
        if (inputShaderType == ShaderType::Undefined) throw std::runtime_error("Input shader's type must not be ShaderType::Undefined");

        // Create shaderc compiler and options
        const shaderc::Compiler compiler = { };
        shaderc::CompileOptions compileOptions = { };

        // Configure compile options
        #if !defined(NDEBUG)
            compileOptions.SetWarningsAsErrors();
            compileOptions.SetGenerateDebugInfo();
        #endif
        compileOptions.SetIncluder(std::make_unique<Includer>(inputShaderFilePath));
        compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
        compileOptions.SetAutoMapLocations(true);
        switch (inputShaderLanguage)
        {
            case InputShaderLanguage::GLSL: { compileOptions.SetSourceLanguage(shaderc_source_language_glsl); compileOptions.SetForcedVersionProfile(450, shaderc_profile_none); break; }
            case InputShaderLanguage::HLSL: { compileOptions.SetSourceLanguage(shaderc_source_language_hlsl); break; }
            default: break;
        }
        compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        compileOptions.SetTargetSpirv(shaderc_spirv_version_1_4);

        // Translate shader kind
        shaderc_shader_kind shaderKind = shaderc_glsl_infer_from_source;
        switch (inputShaderType)
        {
            case ShaderType::Vertex:   { shaderKind = shaderc_vertex_shader;   break; }
            case ShaderType::Fragment: { shaderKind = shaderc_fragment_shader; break; }
            case ShaderType::Compute:  { shaderKind = shaderc_compute_shader;  break; }
            default: break;
        }

        // Compile shader
        const std::vector<char> inputShaderBuffer = File::ReadFile(inputShaderFilePath);
        const shaderc::SpvCompilationResult spirvResult = compiler.CompileGlslToSpv(inputShaderBuffer.data(), inputShaderBuffer.size(), shaderKind, inputShaderFilePath.stem().string().c_str(), compileOptions);
        if (spirvResult.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            printf("An error occurred during SPIR-V generation of shader [%s]: %s\n", inputShaderFilePath.filename().string().c_str(), spirvResult.GetErrorMessage().c_str());
            throw std::runtime_error("SPIR-V generation failed!");
        }

        // Save SPIR-V data
        inputShaderSpirvBuffer = { spirvResult.begin(), spirvResult.end() };
    }

    /* --- POLLING METHODS --- */

    std::filesystem::path CrossShaderCompiler::Compile(const OutputShaderLanguage outputShaderLanguage, const std::filesystem::path &outputShaderDirectory) const
    {
        std::filesystem::path outputShaderFilePath;
        switch (outputShaderLanguage)
        {
            case OutputShaderLanguage::Undefined:
            {
                throw std::runtime_error("Cannot cross compile shader into a OutputShaderLanguage::Undefined!");
                break;
            }
            case OutputShaderLanguage::GLSL:
            {
                outputShaderFilePath = GLSLShaderCompiler(GLSLTargetPlatform::GLSL).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::ESSL:
            {
                outputShaderFilePath = GLSLShaderCompiler(GLSLTargetPlatform::ESSL).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::HLSL:
            {
                outputShaderFilePath = HLSLShaderCompiler().CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::DXIL:
            {
                outputShaderFilePath = HLSLShaderCompiler().CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                outputShaderFilePath = DXILShaderCompiler(inputShaderType).CompileShader(outputShaderFilePath, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::macOSMetalSL:
            {
                outputShaderFilePath = MetalSLShaderCompiler(MetalSLTargetPlatform::macOS).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::iOSMetalSL:
            {
                outputShaderFilePath = MetalSLShaderCompiler(MetalSLTargetPlatform::iOS).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::macOSMetalLib:
            {
                outputShaderFilePath = MetalSLShaderCompiler(MetalSLTargetPlatform::macOS).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                outputShaderFilePath = MetalLibShaderCompiler(MetalLibTargetPlatform::macOS).CompileShader(outputShaderFilePath, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::iOSMetalLib:
            {
                outputShaderFilePath = MetalSLShaderCompiler(MetalSLTargetPlatform::iOS).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                outputShaderFilePath = MetalLibShaderCompiler(MetalLibTargetPlatform::iOS).CompileShader(outputShaderFilePath, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::iOSSimulatorMetalLib:
            {
                outputShaderFilePath = MetalSLShaderCompiler(MetalSLTargetPlatform::iOS).CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                outputShaderFilePath = MetalLibShaderCompiler(MetalLibTargetPlatform::iOSSimulator).CompileShader(outputShaderFilePath, outputShaderDirectory);
                break;
            }
            case OutputShaderLanguage::SPIRV:
            {
                outputShaderFilePath = SPIRVShaderCompiler().CompileShader(inputShaderSpirvBuffer, outputShaderDirectory);
                break;
            }
        }

        return outputShaderFilePath;
    }

    /* --- CONSTRUCTORS --- */

    CrossShaderCompiler::Includer::Includer(std::filesystem::path filePath)
        : filePath(std::move(filePath))
    {

    }

    /* --- GETTER METHODS --- */

    shaderc_include_result* CrossShaderCompiler::Includer::GetInclude(const char* requestedSource, const shaderc_include_type type, const char* requestingSource, const size_t includeDepth)
    {
        // Read included shader data
        auto includeShaderBuffer = File::ReadFile(filePath.parent_path() / requestedSource);

        // Create a container to hold include data (path to source and its file data)
        auto container = new std::array<std::string, 2>();
        (*container)[0] = requestedSource;
        (*container)[1] = std::string(includeShaderBuffer.data(), includeShaderBuffer.size());

        // Assign shaderc's include data
        auto data = new shaderc_include_result();
        data->user_data = container;
        data->source_name = (*container)[0].data();
        data->source_name_length = (*container)[0].size();
        data->content = (*container)[1].data();
        data->content_length = (*container)[1].size();
        return data;
    }

    /* --- DESTRUCTOR --- */

    void CrossShaderCompiler::Includer::ReleaseInclude(shaderc_include_result* data)
    {
        // Deallocate include data container
        delete(reinterpret_cast<std::array<std::string, 2>*>(data->user_data));
        delete(data);
    }

}