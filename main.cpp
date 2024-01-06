#include "src/CrossShaderCompiler.h"

int32 main(const int32 argumentCount, const char* arguments[])
{
    // Validate argument count
    using namespace ShaderConnect;
    if (argumentCount != 6) throw std::runtime_error("Usage: <input_shader_file_path> <input_shader_language['glsl' or 'hlsl']> <input_shader_type['vertex', 'fragment' or 'compute']> <output_shader_directory_path> <output_shader_language['glsl', 'essl', 'hlsl', 'dxil', 'macos-metalsl', 'ios-metalsl', 'macos-metallib', 'ios-metallib', 'ios-simulator-metallib' or 'spir-v']>!");

    // Validate shader file path
    const std::filesystem::path inputShaderFilePath = arguments[1];
    if (!File::FileExists(inputShaderFilePath)) throw std::runtime_error("Specified shader file path is invalid!");

    // Validate input shader language
    const InputShaderLanguage inputShaderLanguage = StringToInputShaderLanguage(arguments[2]);
    if (inputShaderLanguage == InputShaderLanguage::Undefined) throw std::runtime_error("Unrecognized input shader language specified!");

    // Validate shader type
    const ShaderType inputShaderType = StringToShaderType(arguments[3]);
    if (inputShaderType == ShaderType::Undefined) throw std::runtime_error("Unrecognized input shader type specified!");

    // Get output path
    const std::filesystem::path outputShaderFilePath = arguments[4];

    // Validate output shader language
    const OutputShaderLanguage outputShaderLanguage = StringToOutputShaderLanguageLanguage(arguments[5]);
    if (outputShaderLanguage == OutputShaderLanguage::Undefined) throw std::runtime_error("Unrecognized output shader language specified!");

    // Create compiler & compile shader
    const CrossShaderCompiler compiler = CrossShaderCompiler(inputShaderLanguage, inputShaderType, inputShaderFilePath);
    compiler.Compile(outputShaderLanguage, outputShaderFilePath);
}