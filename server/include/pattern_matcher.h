#pragma once

#include <vector>
#include <string>
#include <unordered_map>

/// 
class PatternMatcher
{
public:
    /// 
    explicit PatternMatcher(const std::vector<std::string> &patterns);

    /// 
    std::unordered_map<std::string, size_t> match(const std::string &fileContent);

private:
    std::vector<std::string> checkPatterns; ///< 
};