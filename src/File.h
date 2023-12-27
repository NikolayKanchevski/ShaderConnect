//
// Created by Nikolay Kanchevski on 26.12.23.
//

#pragma once

namespace ShaderConnect
{

    class File
    {
    public:
        /* --- POLLING METHODS --- */
        bool static CreateDirectory(const std::filesystem::path &directoryPath);
        bool static CreateDirectoriesToPath(const std::filesystem::path &path);

        bool static CreateFile(const std::filesystem::path &filePath);
        bool static WriteToFile(const std::filesystem::path &filePath, const char* data, uint64 dataSize, bool overwrite, bool createFile = true);

        /* --- GETTER METHODS --- */
        [[nodiscard]] static bool DirectoryExists(const std::filesystem::path &directoryPath);
        [[nodiscard]] static bool FileExists(const std::filesystem::path &filePath);

        [[nodiscard]] static std::vector<char> ReadFile(const std::filesystem::path &filePath);

    };

}
