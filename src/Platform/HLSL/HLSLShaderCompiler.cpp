//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "HLSLShaderCompiler.h"

#include <spirv_hlsl.hpp>

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    std::filesystem::path HLSLShaderCompiler::CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderDirectory)
    {
        // Set up compiler options
        spirv_cross::CompilerHLSL::Options options = { };
        options.shader_model = 62;
        options.point_coord_compat = false;
        options.support_nonzero_base_vertex_base_instance = false;
        options.force_storage_buffer_as_uav = false;
        options.nonwritable_uav_texture_as_srv = false;
        options.enable_16bit_types = true;
        options.flatten_matrix_vertex_input_semantics = false;
        options.use_entry_point_name = true;
        options.preserve_structured_buffers = true;

        // Create compiler and apply options
        spirv_cross::CompilerHLSL compiler(spirvBuffer);
        compiler.set_hlsl_options(options);

        // Compile shader
        const std::string hlslCode = compiler.compile();
        const std::filesystem::path outputShaderFilePath = outputShaderDirectory / "shader.hlsl";
        File::WriteToFile(outputShaderFilePath, hlslCode.data(), hlslCode.size() * sizeof(char), true, true);

        return outputShaderFilePath;
    }

}