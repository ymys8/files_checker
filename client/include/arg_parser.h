#pragma once

#include <string>
#include <cstdint>

/// Аргументы, полученные из командной строки
struct CmdArgs
{
    std::string filePath; ///< Путь до файла, который необходимо проверить
    uint16_t port;        ///< Порт сервера
};

/// Класс для парсинга аргументов командной строки
class ArgParser
{
public:
    /// Парсинг аргументов командной строки
    static CmdArgs parse(int argc, char* argv[]);
};