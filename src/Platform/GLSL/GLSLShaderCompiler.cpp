//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "GLSLShaderCompiler.h"

#include <spirv_glsl.hpp>

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    void GLSLShaderCompiler::CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderFilePath)
    {
        // Set up compiler options
        spirv_cross::CompilerGLSL::Options options = { };
        options.version = 450;
        options.es = false;
        options.vulkan_semantics = false;
        options.emit_push_constant_as_uniform_buffer = true;
        options.enable_storage_image_qualifier_deduction = true;
        options.force_zero_initialized_variables = false;
        options.force_flattened_io_blocks = false;
        options.relax_nan_checks = false;
        options.enable_row_major_load_workaround = true;
        options.ovr_multiview_view_count = 0;
        options.vertex.fixup_clipspace = false;
        options.vertex.flip_vert_y = false;
        options.fragment.default_float_precision = spirv_cross::CompilerGLSL::Options::Highp;
        options.fragment.default_int_precision = spirv_cross::CompilerGLSL::Options::Highp;

        // Create compiler and apply options
        spirv_cross::CompilerGLSL compiler(spirvBuffer);
        compiler.set_common_options(options);

        // Compile shader
        const std::string glslCode = compiler.compile();
        File::WriteToFile(outputShaderFilePath.string() + ".glsl", glslCode.data(), glslCode.size() * sizeof(char), true, true);
    }

}