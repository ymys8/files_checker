#include "config_reader.h"

#include <fstream>
#include <nlohmann/json.hpp>

std::vector<std::string> ConfigReader::read(const std::string &configPath)
{
    std::ifstream file{configPath};
    if (!file.is_open())
    {
        throw std::runtime_error("Не получилось открыть файл: " + configPath);
    }

    nlohmann::json j;
    try
    {
        file >> j;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw std::runtime_error("Ошибка при парсинге конфига: " + std::string(e.what()));
    }

    if (!j.contains("patterns") || !j["patterns"].is_array())
    {
        throw std::runtime_error("В конфиге должны присутсвовать паттерны (список строк patterns)");
    }

    std::vector<std::string> patterns;
    for (const auto &item : j["patterns"])
    {
        if (!item.is_string())
        {
            throw std::runtime_error("Паттерн должен быть строкой");
        }
        std::string pattern = item.get<std::string>();
        if (pattern.empty())
        {
            throw std::runtime_error("Паттерн не может быть пустым");
        }
        patterns.push_back(pattern);
    }

    if (patterns.empty())
    {
        throw std::runtime_error("Список паттернов пустой");
    }

    return patterns;
}