//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "MetalSLShaderCompiler.h"

#include <spirv_msl.hpp>

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    std::filesystem::path MetalSLShaderCompiler::CompileShader(const std::vector<uint32> &spirvBuffer, const std::filesystem::path &outputShaderFileDirectory)
    {
        // Set up compiler options
        spirv_cross::CompilerMSL::Options options = { };
        switch (targetPlatform)
        {
            case MetalSLTargetPlatform::macOS:
            {
                options.platform = spirv_cross::CompilerMSL::Options::macOS;
                break;
            }
            case MetalSLTargetPlatform::iOS:
            {
                options.platform = spirv_cross::CompilerMSL::Options::iOS;
                break;
            }
        }
        options.msl_version = spirv_cross::CompilerMSL::Options::make_msl_version(2, 0);
        options.texel_buffer_texture_width = 4096;
        options.r32ui_linear_texture_alignment = 4;
        options.r32ui_alignment_constant_id = 65535;
        options.swizzle_buffer_index = 30;
        options.indirect_params_buffer_index = 29;
        options.shader_output_buffer_index = 28;
        options.shader_patch_output_buffer_index = 27;
        options.shader_tess_factor_buffer_index = 26;
        options.buffer_size_buffer_index = 25;
        options.view_mask_buffer_index = 24;
        options.dynamic_offsets_buffer_index = 23;
        options.shader_input_buffer_index = 22;
        options.shader_index_buffer_index = 21;
        options.shader_patch_input_buffer_index = 20;
        options.shader_input_wg_index = 0;
        options.device_index = 0;
        options.enable_frag_output_mask = 0xffffffff;
        options.additional_fixed_sample_mask = 0xffffffff;
        options.enable_point_size_builtin = true;
        options.enable_frag_depth_builtin = true;
        options.enable_frag_stencil_ref_builtin = true;
        options.disable_rasterization = false;
        options.capture_output_to_buffer = false;
        options.swizzle_texture_samples = false;
        options.tess_domain_origin_lower_left = false;
        options.multiview = false;
        options.multiview_layered_rendering = true;
        options.view_index_from_device_index = false;
        options.dispatch_base = false;
        options.texture_1D_as_2D = false;
        options.argument_buffers = false;
        options.argument_buffers_tier = spirv_cross::CompilerMSL::Options::ArgumentBuffersTier::Tier1;
        options.runtime_array_rich_descriptor = false;
        options.enable_base_index_zero = false;
        options.pad_fragment_output_components = false;
        options.ios_support_base_vertex_instance = false;
        options.use_framebuffer_fetch_subpasses = false;
        options.invariant_float_math = false;
        options.emulate_cube_array = true;
        options.enable_decoration_binding = false;
        options.texture_buffer_native = false;
        options.force_active_argument_buffer_resources = false;
        options.pad_argument_buffer_resources = false;
        options.force_native_arrays = true;
        options.enable_clip_distance_user_varying = true;
        options.multi_patch_workgroup = false;
        options.raw_buffer_tese_input = false;
        options.vertex_for_tessellation = false;
        options.arrayed_subpass_input = false;
        options.ios_use_simdgroup_functions = true;
        options.emulate_subgroups = false;
        options.fixed_subgroup_size = 0;
        options.vertex_index_type = spirv_cross::CompilerMSL::Options::IndexType::UInt32;
        options.force_sample_rate_shading = false;
        options.manual_helper_invocation_updates = true;
        options.check_discarded_frag_stores = false;
        options.sample_dref_lod_array_as_grad = false;
        options.readwrite_texture_fences = true;
        options.replace_recursive_inputs = false;
        options.agx_manual_cube_grad_fixup = false;

        // Create compiler and apply options
        spirv_cross::CompilerMSL compiler(spirvBuffer);
        compiler.set_msl_options(options);

        // Compile shader
        const std::string metalSLCode = compiler.compile();
        const std::filesystem::path outputShaderFilePath = outputShaderFileDirectory / (std::string("shader") + (targetPlatform == MetalSLTargetPlatform::macOS ? ".macos" : ".ios") + ".metal");
        File::WriteToFile(outputShaderFilePath, metalSLCode.data(), metalSLCode.size() * sizeof(char), true, true);

        return outputShaderFilePath;
    }

}