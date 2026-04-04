#pragma once

#include <vector>
#include <string>

/// 
class ConfigReader
{
public:
    /// 
    static std::vector<std::string> read(const std::string &configPath);
};