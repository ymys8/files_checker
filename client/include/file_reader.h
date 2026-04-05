#pragma once

#include <string>

/// Класс для чтения файла
class FileReader
{
public:
    /// Чтение файла по переданному пути
    static std::string read(const std::string &filePath);
};