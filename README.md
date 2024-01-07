# Shader Connect

Shader Connect is a lightweight [C++17](https://en.cppreference.com/w/cpp/17) command-line tool, aimed at seamlessly converting [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/)/[HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/) shader code to highly-optimized [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/)/[ESSL](https://www.khronos.org/files/opengles_shading_language.pdf)/[HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/)/[DXIL](https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/DXIL.rst)/[MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf)/[MetalLib](https://developer.apple.com/documentation/metal/shader_libraries/generating_and_loading_a_metal_library_symbol_file?language=objc)/[SPIR-V](www.khronos.org/spir/). It is supported on **Windows**, **macOS** and **Linux**, and can produce shaders,
which are compatible with any operating system, as long as their corresponding graphics API has support for them. 

## 🧰 Usage

Once you have the compiled binary (either after you have built it yourself or downloaded it from [GitHub](https://github.com/NikichaTV/ShaderConnect)), you can run it just as any other command-line application.

The arguments it takes in are as such, respectively:
* `<input_shader_file_path>` - absolute path to the input shader file (it does not have to be of a specific extension, as you manually specify its language) 
* `<input_shader_language>` - language of the input shader file. Can be one of the following:
  * `glsl` [for [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/)]
  * `hlsl` [for [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/)])
* `<input_shader_type>` - what [type/stage](https://www.khronos.org/opengl/wiki/Shader#Stages) the input shader is of. Can be one of the following: 
  * `vertex`
  * `fragment` [also known as [pixel shader](https://www.nvidia.com/en-us/drivers/feature-pixelshader/)]
  * `compute`
* `<output_shader_directory_path>` - absolute path to the output shader *directory, not file*.
* `<output_shader_language>` - language of the output shader file. Can be one of the following: 
  * `spir-v` [for [SPIR-V](www.khronos.org/spir/) binary]
  * `glsl` [for [OpenGL](https://www.khronos.org/opengl/) [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) source]
  * `essl` [[OpenGLES](https://www.khronos.org/opengles/) [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) source]
  * `hlsl` [for [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/) source file]
  * `dxil` [for [DirectX](https://developer.nvidia.com/directx) [DXIL](https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/DXIL.rst) binary file, *supported on Windows only*, requires [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) to be installed]
  * `macos-metalsl` [for macOS [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) source file]
  * `ios-metalsl` [for iOS [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf)] source file)
  * `macos-metallib` [for macOS [MetalLib](https://developer.apple.com/documentation/metal/shader_libraries/generating_and_loading_a_metal_library_symbol_file?language=objc) binary file, *supported on macOS only* requires [Xcode](https://developer.apple.com/xcode/resources/) to be installed)
  * `ios-metallib` [for iOS [MetalLib](https://developer.apple.com/documentation/metal/shader_libraries/generating_and_loading_a_metal_library_symbol_file?language=objc) binary file, *supported on macOS only* requires [Xcode](https://developer.apple.com/xcode/resources/) to be installed)
  * `ios-simulator-metallib` [for iOS simulator ([Xcode](https://developer.apple.com/xcode/)'s emulator) [MetalLib](https://developer.apple.com/documentation/metal/shader_libraries/generating_and_loading_a_metal_library_symbol_file?language=objc) binary file, *supported on macOS only*)

<br>

#### Example usage:
```bat
$ ShaderConnect.exe C:\Users\MyUser\Shaders\input_shader.vert glsl vertex C:\Users\MyUser\Shaders\Generated\ spir-v   
```

After running the command, if all the arguments are valid, you are going to see a new shader file in the specified`<output_shader_directory_path>` directory. 

<br>

## 🤔 ShaderConnect

#### Why was  it developed?
Being the lead developer of the [Sierra Engine](https://github.com/NikichaTV/SierraEngine) (a cross-platform *multi-API* rendering engine), I quickly found myself needing to write numerous versions of every shader I had
just to satisfy the requirements of the various graphics APIs the engine was built to seamlessly support. For example, [Vulkan](https://www.vulkan.org) works with [SPIR-V](www.khronos.org/spir/) binary shaders, [DirectX](https://developer.nvidia.com/directx) uses intermediate format shaders (compiled from [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/)), [Metal](https://developer.apple.com/metal/) needs C++-like [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf)
text shaders, which are ultimately compiled down to binary, and [OpenGL](https://www.khronos.org/opengl/) uses plain [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) text shaders.

So, I could either keep on manually writing shaders in all the formats I just mentioned (possibly even more in the future, provided that support for new API was to come), or I could find me a way to convert the single shader format I
write in to the rest, and, given that you are checking out this repository, you too are most probably looking for a solution to that same problem... or are simply too lazy to learn a new shading language. Either way, look no more!

#### What *exactly* is ShaderConnect?
It is command line tool, which glues together one or more already-existing shader compilers and converters, which allows for the "cross compilation". It is *not* a compiler in of itself by any means.

The way it works is fairly simple: the program takes in a path to a shader, which must be in either [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) or [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/) format, this shader is then compiled down to [SPIR-V](www.khronos.org/spir/) binary using [Shaderc](https://github.com/google/shaderc). Finally, thanks to [SPIR-V Cross](https://github.com/KhronosGroup/SPIRV-Cross), the binary gets translated into
a source file of the desired output language.

<br>

## 📝 Test Input

We will be using the following [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) shader as a sample to convert to all supported output shading languages:
```glsl
#version 450

layout(location = 0) out vec3 toFrag_Color;

vec2 positions[3] = vec2[](
    vec2(-0.5, -0.5),
    vec2( 0.5, -0.5),
    vec2( 0.0,  0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main()
{
    gl_Position = vec4(positions[gl_VertexIndex].xy, 0.0, 1.0);
    toFrag_Color = colors[gl_VertexIndex];
}
```

#### Generated [ESSL](https://www.khronos.org/files/opengles_shading_language.pdf):
```glsl
#version 450 es

const vec2 _19[3] = vec2[](vec2(-0.5), vec2(0.5, -0.5), vec2(0.0, 0.5));
const vec3 _28[3] = vec3[](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));

layout(location = 0) out vec3 _49;

void main()
{
    gl_Position = vec4(_19[gl_VertexID], 0.0, 1.0);
    _49 = _28[gl_VertexID];
}
```
It is pretty much the same, however, if there are any [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/)-only features, they will be emulated using [ESSL](https://www.khronos.org/files/opengles_shading_language.pdf) core features if possible.

### Generated [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/):
```c++
    static const float2 _19[3] = { (-0.5f).xx, float2(0.5f, -0.5f), float2(0.0f, 0.5f) };
    static const float3 _28[3] = { float3(1.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, 0.0f, 1.0f) };
    
    static float4 gl_Position;
    static int gl_VertexIndex;
    static float3 _49;
    
    struct SPIRV_Cross_Input
    {
        uint gl_VertexIndex : SV_VertexID;
    };
    
    struct SPIRV_Cross_Output
    {
        float3 _49 : TEXCOORD0;
        float4 gl_Position : SV_Position;
    };
    
    void main_inner()
    {
        gl_Position = float4(_19[gl_VertexIndex], 0.0f, 1.0f);
        _49 = _28[gl_VertexIndex];
    }
    
    SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
    {
        gl_VertexIndex = int(stage_input.gl_VertexIndex);
        main_inner();
        SPIRV_Cross_Output stage_output;
        stage_output.gl_Position = gl_Position;
        stage_output._49 = _49;
        return stage_output;
    }
```
Perfect!

### Generated [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) (iOS & macOS)
```c++
#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

constant float2 _19[3] = { float2(-0.5), float2(0.5, -0.5), float2(0.0, 0.5) };
constant float3 _28[3] = { float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0) };

struct main0_out
{
    float3 m_49 [[user(locn0)]];
    float4 gl_Position [[position]];
};

vertex main0_out main0(uint gl_VertexIndex [[vertex_id]])
{
    main0_out out = {};
    out.gl_Position = float4(_19[int(gl_VertexIndex)], 0.0, 1.0);
    out.m_49 = _28[int(gl_VertexIndex)];
    return out;
}
```
In this case both the iOS and macOS [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) are identical, but, just like with [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/)/[ESSL](https://www.khronos.org/files/opengles_shading_language.pdf), if iOS [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) lacks some feature present in the macOS version, it will be emulated with another core features where possible.
Specifying the correct target platform also allows for better optimization.

### Generated [SPIR-V](www.khronos.org/spir/)
While displaying a binary format in a human-readable text format is not really possible, here is what the generated [SPIR-V](www.khronos.org/spir/) looks when ran through the [SPIR-V Visualizer](https://www.khronos.org/spir/visualizer/)'s parser:
```
Mode Setting
[0] OpCapability Shader
[1] %1 = OpExtInstImport "[GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/).std.450"
[2] OpMemoryModel Logical [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/)450
[3] OpEntryPoint Vertex %4 "main" %34 %38 %49
Annotations
[4] OpMemberDecorate %32 0 BuiltIn Position
[5] OpMemberDecorate %32 1 BuiltIn PointSize
[6] OpMemberDecorate %32 2 BuiltIn ClipDistance
[7] OpMemberDecorate %32 3 BuiltIn CullDistance
[8] OpDecorate %32 Block
[9] OpDecorate %38 BuiltIn VertexIndex
[10] OpDecorate %49 Location 0
Types, variables and constants
[11] %2 = OpTypeVoid
[12] %3 = OpTypeFunction %2
[13] %6 = OpTypeFloat 32
[14] %7 = OpTypeVector %6 2
[15] %8 = OpTypeInt 32 0
[16] %9 = OpConstant %8 3
[17] %10 = OpTypeArray %7 %9
[18] %13 = OpConstant %6 -0.5f
[19] %14 = OpConstantComposite %7 %13 %13
[20] %15 = OpConstant %6 0.5f
[21] %16 = OpConstantComposite %7 %15 %13
[22] %17 = OpConstant %6 0.0f
[23] %18 = OpConstantComposite %7 %17 %15
[24] %19 = OpConstantComposite %10 %14 %16 %18
[25] %20 = OpTypeVector %6 3
[26] %21 = OpTypeArray %20 %9
[27] %24 = OpConstant %6 1.0f
[28] %25 = OpConstantComposite %20 %24 %17 %17
[29] %26 = OpConstantComposite %20 %17 %24 %17
[30] %27 = OpConstantComposite %20 %17 %17 %24
[31] %28 = OpConstantComposite %21 %25 %26 %27
[32] %29 = OpTypeVector %6 4
[33] %30 = OpConstant %8 1
[34] %31 = OpTypeArray %6 %30
[35] %32 = OpTypeStruct %29 %6 %31 %31
[36] %33 = OpTypePointer Output %32
[37] %34 = OpVariable %33 Output
[38] %35 = OpTypeInt 32 1
[39] %36 = OpConstant %35 0
[40] %37 = OpTypePointer Input %35
[41] %38 = OpVariable %37 Input
[42] %46 = OpTypePointer Output %29
[43] %48 = OpTypePointer Output %20
[44] %49 = OpVariable %48 Output
[45] %54 = OpTypePointer Function %10
[46] %55 = OpTypePointer Function %7
[47] %56 = OpTypePointer Function %21
[48] %57 = OpTypePointer Function %20
Function 49
[49] %4 = OpFunction %2 None %3
Label 50
[Return 50]
[50] %5 = OpLabel
[51] %23 = OpVariable %56 Function
[52] %12 = OpVariable %54 Function
[53] OpStore %12 %19
[54] OpStore %23 %28
[55] %39 = OpLoad %35 %38
[56] %41 = OpAccessChain %55 %12 %39
[57] %42 = OpLoad %7 %41
[58] %43 = OpCompositeExtract %6 %42 0
[59] %44 = OpCompositeExtract %6 %42 1
[60] %45 = OpCompositeConstruct %29 %43 %44 %17 %24
[61] %47 = OpAccessChain %46 %34 %36
[62] OpStore %47 %45
[63] %52 = OpAccessChain %57 %23 %39
[64] %53 = OpLoad %20 %52
[65] OpStore %49 %53
[66] OpReturn
[67] OpFunctionEnd
```

### Generated [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/):
While nobody in the right mind will ever need to translate a shader from one language to that same one, it is useful to allow this to test wether any data is lost during the cross compilation. As you can see, everything stays exactly the same:
```glsl
#version 450

const vec2 _19[3] = vec2[](vec2(-0.5), vec2(0.5, -0.5), vec2(0.0, 0.5));
const vec3 _28[3] = vec3[](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));

layout(location = 0) out vec3 _49;

void main()
{
    gl_Position = vec4(_19[gl_VertexID], 0.0, 1.0);
    _49 = _28[gl_VertexID];
}
```

<br>

## 🚫 Limitations

What [ShaderConnect](https://github.com/NikichaTV/ShaderConnect) is trying to do is seamlessly satisfy numerous APIs, some of which completely different from one another, but this is never possible without at least a few compromises, all of which are conveniently listed here:

<br>

- Input shader must be in either [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) or [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/) format.
- Version of [GLSL](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)/) and [ESSL](https://www.khronos.org/files/opengles_shading_language.pdf) output shaders is 450.
- The shader model version of [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/) and [DXIL](https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/DXIL.rst) output shaders is 6.6.
- [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) version of both macOS [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) and iOS [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) output shaders is 2.0.
- Generated [SPIR-V](www.khronos.org/spir/) is in [SPIR-V](www.khronos.org/spir/) 1.4.
- If your target language allows custom entry point name, it must always be called "main".
- A method named "main" will get renamed to "main0" when compiling [MetalSL](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf) (otherwise an error caused by confusion with the built-in main symbol may arise). 
- Output channel count in fragment/pixel shader **must** match that of the corresponding render pass and/or pipeline it is used within.
- You **must** use 32-bit (`uint32_t`) [index buffers](https://learn.microsoft.com/en-us/windows/uwp/graphics-concepts/index-buffers) with vertex shaders.
- [Vulkan](https://www.vulkan.org) [push constants](https://docs.vulkan.org/guide/latest/push_constants.html) are converted to [uniform buffers](https://www.khronos.org/opengl/wiki/Uniform_Buffer_Object) in **all** output languages **except for** [SPIR-V](www.khronos.org/spir/) (where they remain unchanged) **and** [HLSL](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl/) (where they are converted to [root constants](https://learn.microsoft.com/en-us/windows/win32/direct3d12/root-signatures)).

There are probably more, however, the project is still fairly new and has not been tested thoroughly yet. Feel free to open an issue at any time if something is concerning you.

<br>

## 🤓️ About

**Frameworks used:**

* [Shaderc](https://github.com/google/shaderc/) - Used to compile input shader code to binary [SPIR-V](www.khronos.org/spir/).
* [SPIR-V Cross](https://github.com/KhronosGroup/SPIRV-Cross/) - Translates [SPIR-V](www.khronos.org/spir/) data to a high-level shading language.
* [DXC](https://github.com/microsoft/DirectXShaderCompiler/) - Official [DirectX](https://developer.nvidia.com/directx/) compiler for [DXIL](https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/DXIL.rst).

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

<p align="center" id="LineCounter">Total lines of code: 1,674</p>
<p align="center" id="LastUpdated">Last updated: 07/01/2024 </p>

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------