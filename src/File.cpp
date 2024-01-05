//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "File.h"

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    bool File::CreateFile(const std::filesystem::path &filePath)
    {
        if (FileExists(filePath)) return true;
        if (!CreateDirectory(filePath.parent_path())) return false;

        // Try to open file (stream creates a file if missing)
        std::ofstream file(filePath);
        if (!file.is_open()) return false;
        file.close();

        return true;
    }

    bool File::DeleteFile(const std::filesystem::path &filePath)
    {
        if (!FileExists(filePath)) return true;
        return remove(filePath.string().c_str()) == 0;
    }

    bool File::CreateDirectory(const std::filesystem::path &directoryPath)
    {
        if (DirectoryExists(directoryPath)) return true;
        return std::filesystem::create_directories(directoryPath);
    }

    bool File::DeleteDirectory(const std::filesystem::path &directoryPath)
    {
        if (!DirectoryExists(directoryPath)) return true;
        return std::filesystem::remove(directoryPath);
    }

    std::vector<char> File::ReadFile(const std::filesystem::path &filePath)
    {
        if (!FileExists(filePath)) throw std::runtime_error("Could not open file for reading, as it does not exist!");

        // Try to create a stream to read file
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) throw std::runtime_error("Could not open file for reading!");

        // Get file size and create resized vector
        const std::streamsize fileSize = static_cast<int64>(file.tellg());
        std::vector<char> fileBuffer(fileSize);

        // Sava file data to buffer
        file.seekg(0);
        file.read(fileBuffer.data(), fileSize);

        // Close file and return data
        file.close();
        return fileBuffer;
    }

    bool File::WriteToFile(const std::filesystem::path &filePath, const char* data, const uint64 dataSize, const bool overwrite, const bool createFile)
    {
        if (createFile) CreateFile(filePath);
        if (!FileExists(filePath)) return false;

        // Write data to file
        std::ofstream file(filePath, std::ios::out | std::ios::binary | (overwrite ? std::ios::trunc : std::ios::app));
        if (!file.is_open()) return false;

        // Write to file and close
        file.write(data, static_cast<std::streamsize>(dataSize));
        file.close();

        return true;
    }

    /* --- GETTER METHODS --- */

    bool File::FileExists(const std::filesystem::path &filePath)
    {
        return std::filesystem::exists(filePath);
    }

    bool File::DirectoryExists(const std::filesystem::path &directoryPath)
    {
        return std::filesystem::exists(directoryPath);
    }

}