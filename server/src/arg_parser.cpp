#include "arg_parser.h"

#include <unistd.h>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

const std::string howUseUtil = "Как использовать: server -c <путь до конфига> -p <порт сервера>";

CmdArgs ArgParser::parse(int argc, char *argv[])
{
    std::string configPath;
    std::string portStr;

    int opt;
    while ((opt = getopt(argc, argv, "c:p:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            configPath = optarg;
            break;
        case 'p':
            portStr = optarg;
            break;
        default:
            throw std::invalid_argument(howUseUtil);
        }
    }

    if (configPath.empty() || portStr.empty())
    {
        throw std::invalid_argument(howUseUtil);
    }

    // Проверка пути до файла
    fs::path path{configPath};
    if (!fs::exists(path))
    {
        throw std::invalid_argument("Файл не найден: " + configPath);
    }
    if (!fs::is_regular_file(path))
    {
        throw std::invalid_argument("Выбран не обычный файл конфига: " + configPath);
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

    return CmdArgs{configPath, static_cast<uint16_t>(port)};
}