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
        options.argument_buffers = true;
        options.argument_buffers_tier = spirv_cross::CompilerMSL::Options::ArgumentBuffersTier::Tier2;
        options.runtime_array_rich_descriptor = false;
        options.enable_base_index_zero = false;
        options.pad_fragment_output_components = false;
        options.ios_support_base_vertex_instance = false;
        options.use_framebuffer_fetch_subpasses = false;
        options.invariant_float_math = false;
        options.emulate_cube_array = true;
        options.enable_decoration_binding = false;
        options.texture_buffer_native = false;
        options.force_active_argument_buffer_resources = true;
        options.pad_argument_buffer_resources = true;
        options.force_native_arrays = false;
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

        constexpr uint32 UNIFORM_BUFFER_BINDING         = 0;
        constexpr uint32 STORAGE_BUFFER_BINDING         = 1;
        constexpr uint32 SAMPLED_IMAGE_BINDING          = 2;
        constexpr uint32 SAMPLED_CUBEMAP_BINDING        = 3;
        constexpr uint32 STORAGE_IMAGE_BINDING          = 4;
        constexpr uint32 STORAGE_CUBEMAP_BINDING        = 5;
        constexpr uint32 SAMPLER_BINDING                = 6;

        /* === Reference: https://developer.apple.com/metal/Metal-Feature-Set-Tables.pdf === */
        constexpr static uint32 UNIFORM_BUFFER_CAPACITY         = 500'000;
        constexpr static uint32 STORAGE_BUFFER_CAPACITY         = 500'000;
        constexpr static uint32 SAMPLED_IMAGE_CAPACITY          = 250'000;
        constexpr static uint32 SAMPLED_CUBEMAP_CAPACITY        = 250'000;
        constexpr static uint32 STORAGE_IMAGE_CAPACITY          = 250'000;
        constexpr static uint32 STORAGE_CUBEMAP_CAPACITY        = 250'000;
        constexpr static uint32 SAMPLER_CAPACITY                = 1024;

        // Define bindless layout (NOTE: for some reason SPIRV-Cross' padding breaks unless capacity is split into chunks of multiples of 250'000)
        constexpr std::array<spirv_cross::MSLResourceBinding, 9> BINDINGS
        {
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Void,
                .desc_set = 0,
                .binding = UNIFORM_BUFFER_BINDING,
                .count = UNIFORM_BUFFER_CAPACITY / 2
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Void,
                .desc_set = 0,
                .binding = UNIFORM_BUFFER_BINDING,
                .count = UNIFORM_BUFFER_CAPACITY / 2
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Void,
                .desc_set = 0,
                .binding = STORAGE_BUFFER_BINDING,
                .count = STORAGE_BUFFER_CAPACITY / 2
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Void,
                .desc_set = 0,
                .binding = STORAGE_BUFFER_BINDING,
                .count = STORAGE_BUFFER_CAPACITY / 2
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::SampledImage,
                .desc_set = 0,
                .binding = SAMPLED_IMAGE_BINDING,
                .count = SAMPLED_IMAGE_CAPACITY
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::SampledImage,
                .desc_set = 0,
                .binding = SAMPLED_CUBEMAP_BINDING,
                .count = SAMPLED_CUBEMAP_CAPACITY
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Image,
                .desc_set = 0,
                .binding = STORAGE_IMAGE_BINDING,
                .count = STORAGE_IMAGE_CAPACITY
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Image,
                .desc_set = 0,
                .binding = STORAGE_CUBEMAP_BINDING,
                .count = STORAGE_CUBEMAP_CAPACITY
            },
            spirv_cross::MSLResourceBinding {
                .basetype = spirv_cross::SPIRType::Sampler,
                .desc_set = 0,
                .binding = SAMPLER_BINDING,
                .count = SAMPLER_CAPACITY
            }
        };

        // Add bindings to every shader type
        uint32 currentIndex = 0;
        compiler.set_argument_buffer_device_address_space(0, true);
        for (spirv_cross::MSLResourceBinding binding : BINDINGS)
        {
            binding.msl_buffer = currentIndex;
            binding.msl_texture = currentIndex;
            binding.msl_sampler = currentIndex;

            binding.stage = spv::ExecutionModelVertex;
            compiler.add_msl_resource_binding(binding);

            binding.stage = spv::ExecutionModelFragment;
            compiler.add_msl_resource_binding(binding);

            binding.stage = spv::ExecutionModelKernel;
            compiler.add_msl_resource_binding(binding);

            currentIndex += binding.count;
        }
        
        // Compile shader
        const std::string metalSLCode = compiler.compile();
        const std::filesystem::path outputShaderFilePath = outputShaderFileDirectory / (std::string("shader") + (targetPlatform == MetalSLTargetPlatform::macOS ? ".macos" : ".ios") + ".metal");
        File::WriteToFile(outputShaderFilePath, metalSLCode.data(), metalSLCode.size() * sizeof(char), true, true);

        return outputShaderFilePath;
    }

}