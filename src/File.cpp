//
// Created by Nikolay Kanchevski on 26.12.23.
//

#include "File.h"

namespace ShaderConnect
{

    /* --- POLLING METHODS --- */

    bool File::CreateDirectory(const std::filesystem::path &directoryPath)
    {
        if (DirectoryExists(directoryPath)) return true;

        #if defined _MSC_VER
            return _mkdir(directoryPath.c_str()) == 0;
        #elif defined __GNUC__
            return mkdir(directoryPath.c_str(), 0777) == 0;
        #endif
    }

    bool File::CreateDirectoriesToPath(const std::filesystem::path &path)
    {
        // Create every subdirectory
        for (const auto &directory : path)
        {
            if (!CreateDirectory(relative(directory, directory.parent_path()))) return false;
        }

        return true;
    }

    bool File::CreateFile(const std::filesystem::path &filePath)
    {
        if (FileExists(filePath)) return true;
        if (!CreateDirectoriesToPath(filePath)) return false;

        // Try to open file (ofstream creates a file if missing)
        std::ofstream file(filePath);
        if (!file.is_open()) return false;
        file.close();

        return true;
    }

    bool File::WriteToFile(const std::filesystem::path &filePath, const char* data, const uint64 dataSize, bool overwrite, bool createFile)
    {
        if (createFile) CreateFile(filePath);

        if (!FileExists(filePath))
        {
            printf("Warning: Trying to write data to a file with a path of [%s] but the file does not exist!\n", filePath.c_str());
            return false;
        }

        // Write data to file
        std::ofstream file(filePath, std::ios::out | std::ios::binary | (overwrite ? std::ios::trunc : std::ios::app));
        if (!file.is_open()) return false;

        // Write to file and close
        file.write(data, static_cast<std::streamsize>(dataSize));
        file.close();

        return true;
    }

    /* --- GETTER METHODS --- */

    bool File::DirectoryExists(const std::filesystem::path &directoryPath)
    {
        struct stat buffer = { };
        return stat(directoryPath.c_str(), &buffer) == 0;
    }

    bool File::FileExists(const std::filesystem::path &filePath)
    {
        struct stat buffer = { };
        return stat(filePath.c_str(), &buffer) == 0;
    }

    std::vector<char> File::ReadFile(const std::filesystem::path &filePath)
    {
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

}