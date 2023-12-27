cmake_minimum_required(VERSION 3.27)
project(ShaderConnect)

set(CMAKE_CXX_STANDARD 17)

# Create executable
add_executable(${PROJECT_NAME} main.cpp src/ShaderType.h src/InputShaderLanguage.h src/CrossShaderCompiler.h src/CrossShaderCompiler.cpp src/OutputShaderLanguage.h src/ShaderCompiler.h src/File.cpp src/File.h src/Platform/GLSL/GLSLShaderCompiler.cpp src/Platform/GLSL/GLSLShaderCompiler.h src/Platform/ESSL/ESSLShaderCompiler.cpp src/Platform/ESSL/ESSLShaderCompiler.h src/Platform/HLSL/HLSLShaderCompiler.cpp src/Platform/HLSL/HLSLShaderCompiler.h src/Platform/macOS-MSL/macOSMSLShaderCompiler.cpp src/Platform/macOS-MSL/macOSMSLShaderCompiler.h src/Platform/iOS-MSL/iOSMSLShaderCompiler.cpp src/Platform/iOS-MSL/iOSMSLShaderCompiler.h src/Platform/SPIR-V/SPIRVShaderCompiler.cpp src/Platform/SPIR-V/SPIRVShaderCompiler.h)

# Include shaderc dependencies
set(SPIRV_WERROR OFF)
add_subdirectory(vendor/SPIRV-Tools)
set(GLSLANG_TESTS OFF)
add_subdirectory(vendor/glslang)
set(BUILD_TESTS OFF)
add_subdirectory(vendor/SPIRV-Headers)

# Link shaderc
set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_EXAMPLES ON)
add_subdirectory(vendor/shaderc)
target_link_libraries(${PROJECT_NAME} PRIVATE shaderc)

# Link SPIR-V Cross
set(SPIRV_CROSS_ENABLE_TESTS OFF)
set(SPIRV_CROSS_ENABLE_C_API OFF)
add_subdirectory(vendor/SPIRV-Cross)
target_link_libraries(${PROJECT_NAME} PRIVATE spirv-cross-cpp spirv-cross-glsl spirv-cross-hlsl spirv-cross-msl)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/SPIRV-Cross)

# Link precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE src/scpch.h)

# Run update script
find_package(Python)
if(Python_FOUND)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/UpdateProject.py)
    message(STATUS "[ShaderConnect]: Running project update scripts...")
else()
    message(WARNING "[ShaderConnect]: Python install was not found on the machine. Project update scripts cannot be run!")
endif()
