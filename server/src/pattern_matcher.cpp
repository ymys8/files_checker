#include "pattern_matcher.h"

PatternMatcher::PatternMatcher(const std::vector<std::string> &patterns) : checkPatterns(patterns)
{
}

std::unordered_map<std::string, size_t> PatternMatcher::match(const std::string &fileContent)
{
    std::unordered_map<std::string, size_t> matchResult;
    for (const auto &pattern : checkPatterns)
    {
        size_t pos = 0;
        while ((pos = fileContent.find(pattern, pos)) != std::string::npos)
        {
            matchResult[pattern]++;
            pos += pattern.size();
        }
    }

    return matchResult;
}