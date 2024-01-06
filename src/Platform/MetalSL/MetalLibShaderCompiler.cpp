//
// Created by Nikolay Kanchevski on 4.01.24.
//

#include "MetalLibShaderCompiler.h"

namespace ShaderConnect
{

    /* --- CONSTRUCTORS --- */

    /* --- POLLING METHODS --- */

    std::filesystem::path MetalLibShaderCompiler::CompileShader(const std::filesystem::path &inputMetalShaderFilePath, const std::filesystem::path &outputShaderDirectory)
    {
        #if !SC_PLATFORM_macOS
            throw std::runtime_error("Cannot cross-compile shaders into Metal libraries (.metallib), unless host is running macOS!");
        #endif
        if (!File::FileExists(inputMetalShaderFilePath)) throw std::runtime_error("Cannot compile Metal shader [" + inputMetalShaderFilePath.string() + "] into a Metal library, as it does not exist!");

        // Choose SDK
        bool isTargetSimulator = false;
        std::string sdkName = "Unknown";
        switch (targetPlatform)
        {
            case MetalLibTargetPlatform::macOS:
            {
                sdkName = "macosx";
                break;
            }
            case MetalLibTargetPlatform::iOS:
            {
                sdkName = "iphoneos";
                break;
            }
            case MetalLibTargetPlatform::iOSSimulator:
            {
                sdkName = "iphonesimulator";
                isTargetSimulator = true;
                break;
            }
        }

        // Define shader paths
        const std::filesystem::path airFilePath = std::filesystem::path(inputMetalShaderFilePath).replace_extension(".air");
        const std::filesystem::path outputShaderFilePath = outputShaderDirectory / (inputMetalShaderFilePath.stem().string()  + (isTargetSimulator ? "-simulator" : "") + ".metallib");

        // Create intermediate binary
        const std::string intermediateCommand = "xcrun -sdk " + sdkName + " metal -c " + inputMetalShaderFilePath.string() + " -o " + airFilePath.string();
        printf("%s", intermediateCommand.c_str());
        system(intermediateCommand.c_str());

        // Compile binary to Metal library
        const std::string compileCommand = "xcrun -sdk " + sdkName + " metallib " + airFilePath.string() + " -o " + outputShaderFilePath.string();
        printf("%s", compileCommand.c_str());
        system(compileCommand.c_str());

        // Delete temporary files
        File::DeleteFile(inputMetalShaderFilePath);
        File::DeleteFile(airFilePath);

        return outputShaderFilePath;
    }

    /* --- SETTER METHODS --- */

    /* --- GETTER METHODS --- */

    /* --- DESTRUCTOR --- */

}