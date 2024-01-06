//
// Created by Nikolay Kanchevski on 1.6.2024.
//

#include "DXILShaderCompiler.h"

#if SC_PLATFORM_WINDOWS
    #include <d3d12shader.h>
    #include <dxcapi.h>
    #include <dxcerrors.h>
    #include <dxcisense.h>
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #undef DeleteFile
#endif

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    std::filesystem::path DXILShaderCompiler::CompileShader(const std::filesystem::path &inputHLSLShaderFilePath, const std::filesystem::path &outputShaderDirectory)
    {
        #if !SC_PLATFORM_WINDOWS
            throw std::runtime_error("Cannot cross-compile shaders into DirectX intermediate language (.dxil), unless host is running Windows!");
            return { };
        #else
            if (!File::FileExists(inputHLSLShaderFilePath)) throw std::runtime_error("Cannot compile HLSL shader [" + inputHLSLShaderFilePath.string() + "] into a DXIL, as it does not exist!");
            const std::vector<char> hlslData = File::ReadFile(inputHLSLShaderFilePath);

            // Define shader path
            const std::filesystem::path outputShaderFilePath = outputShaderDirectory / (inputHLSLShaderFilePath.stem().string() + ".dxil");

            // Determine shader profile
            std::wstring shaderProfile = L"Undefined";
            switch (shaderType)
            {
                case ShaderType::Vertex:      { shaderProfile = L"vs_6_6"; break; }
                case ShaderType::Fragment:    { shaderProfile = L"ps_6_6"; break; }
                case ShaderType::Compute:     { shaderProfile = L"cs_6_6"; break; }
                default:                      break;
            }

            // Create library instance
            IDxcLibrary* library = nullptr;
            HRESULT result = DxcCreateInstance(CLSID_DxcLibrary, __uuidof(IDxcLibrary), reinterpret_cast<void**>(&library));
            if (FAILED(result)) throw std::runtime_error("Could not compile HLSL shader into DXIL, as the creation of a DXC library instance failed!");

            // Create compiler
            IDxcCompiler* compiler = nullptr;
            result = DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler), reinterpret_cast<void**>(&compiler));
            if (FAILED(result)) throw std::runtime_error("Could not compile HLSL shader into DXIL, as the creation of a DXC compiler instance failed!");

            // Create shader blob
            IDxcBlobEncoding* shaderSource;
            result = library->CreateBlobWithEncodingOnHeapCopy(hlslData.data(), static_cast<uint32>(hlslData.size() * sizeof(char)), CP_UTF8, &shaderSource);
            if (FAILED(result)) throw std::runtime_error("Could not compile HLSL shader into DXIL, as the creation of a DXC blob for it failed!");

            // Compile shader
            std::array<LPCWSTR, 2> ARGUMENTS = { L"-Qstrip_debug", L"-Qstrip_reflect" };
            IDxcOperationResult* compileResult = nullptr;
            result = compiler->Compile(shaderSource, L"Input Shader", L"main", shaderProfile.c_str(), ARGUMENTS.data(), static_cast<uint32>(ARGUMENTS.size()), nullptr, 0, nullptr, &compileResult);
            if (FAILED(result)) throw std::runtime_error("Could not compile HLSL shader into DXIL, as the compilation failed!");
            else
            {
                // Get compiled shader data
                IDxcBlob* outputData = nullptr;
                compileResult->GetResult(&outputData);

                // Write it to file
                File::WriteToFile(outputShaderFilePath, reinterpret_cast<char*>(outputData->GetBufferPointer()), outputData->GetBufferSize(), true, true);
                outputData->Release();
            }

            // Log errors
            IDxcBlobEncoding* errorBuffer = nullptr;
            compileResult->GetErrorBuffer(&errorBuffer);
            if (errorBuffer->GetBufferSize() > 0) printf("%s\n", reinterpret_cast<char*>(errorBuffer->GetBufferPointer()));

            // Release debug info
            errorBuffer->Release();
            compileResult->Release();

            // Release objects
            shaderSource->Release();
            compiler->Release();
            library->Release();

            // Delete temporary HLSL file
            File::DeleteFile(inputHLSLShaderFilePath);
            return outputShaderFilePath;
        #endif
    }

}