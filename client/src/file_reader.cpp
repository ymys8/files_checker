#include "file_reader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string FileReader::read(const std::string &filePath)
{
    std::ifstream file{filePath};
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл: " + filePath);
    }

    std::string fileContent;
    std::string line;
    while (std::getline(file, line))
    {
        line.append("\n");
        fileContent.append(line);
    }

    if (file.fail() && !file.eof())
    {
        throw std::runtime_error("Ошибка при чтении файла: " + filePath);
    }

    return fileContent;
}