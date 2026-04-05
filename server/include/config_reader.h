#pragma once

#include <vector>
#include <string>

/// Класс для чтения конфига
class ConfigReader
{
public:
    /// Прочитать паттерны из конфига
    static std::vector<std::string> read(const std::string &configPath);
};