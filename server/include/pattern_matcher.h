#pragma once

#include <vector>
#include <string>
#include <unordered_map>

/// Класс посика паттернов в тексте
class PatternMatcher
{
public:
    /// Конструктор
    explicit PatternMatcher(const std::vector<std::string> &patterns);

    /// Проверка текста по всем заданным паттернам
    std::unordered_map<std::string, size_t> match(const std::string &fileContent);

private:
    std::vector<std::string> checkPatterns; ///< Паттерны для проверки текста
};