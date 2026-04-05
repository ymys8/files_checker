#pragma once

#include <string>
#include <cstdint>

/// Аргументы командной строки
struct CmdArgs
{
    std::string configPath; ///< Путь до конфига
    uint16_t port;          ///< Порт для сервера
};

/// Класс парсинга командной строки
class ArgParser
{
public:
    /// Парсинг аргументов командной строки
    static CmdArgs parse(int argc, char *argv[]);
};
