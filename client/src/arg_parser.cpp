#include "arg_parser.h"

#include <unistd.h>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

const std::string howUseUtil = "Как использовать: client -f <путь до файла> -p <порт сервера>";

CmdArgs ArgParser::parse(int argc, char *argv[])
{
    std::string filePath;
    std::string portStr;

    int opt;
    while ((opt = getopt(argc, argv, "f:p:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            filePath = optarg;
            break;
        case 'p':
            portStr = optarg;
            break;
        default:
            throw std::invalid_argument(howUseUtil);
        }
    }

    if (filePath.empty() || portStr.empty())
    {
        throw std::invalid_argument(howUseUtil);
    }

    // Проверка пути до файла
    fs::path path{filePath};
    if (!fs::exists(path))
    {
        throw std::invalid_argument("Файл не найден: " + filePath);
    }
    if (!fs::is_regular_file(path))
    {
        throw std::invalid_argument("Выьран не обычный файл: " + filePath);
    }

    // Проверка порта
    int port = 0;
    try
    {
        std::size_t pos = 0;
        port = std::stoi(portStr, &pos);
        if (pos != portStr.size())
        {
            throw std::invalid_argument("");
        }
    }
    catch (const std::exception &)
    {
        throw std::invalid_argument("Порт должен содержать только числа: " + portStr);
    }

    if (port < 1 || port > 65535)
    {
        throw std::invalid_argument(
            "Порт должен быть на отрезке от 1 до 65535: " + portStr);
    }

    return CmdArgs{filePath, static_cast<uint16_t>(port)};
}