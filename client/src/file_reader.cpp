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

    std::ostringstream ss;
    ss << file.rdbuf();

    if (file.bad())
    {
        throw std::runtime_error("Ошибка при чтении файла: " + filePath);
    }

    return ss.str();
}