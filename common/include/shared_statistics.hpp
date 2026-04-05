#pragma once

#include <cstddef>

const int MAX_PATTERNS = 64;
const int MAX_PATTERN_LEN = 128;

inline const char* FIFO_PATH = "/tmp/hometask.fifo";

/// Статистика по конкретному паттерну
struct PatternStatistic
{
    char name[MAX_PATTERN_LEN]; ///< Имя паттерна
    size_t matchCount;          ///< Количество обнаружений паттерна
};

/// Статистика по всем проверкам
struct SharedStatistic
{
    size_t checkedFilesCount;                ///< Количество проверенных файлов
    size_t patternCount;                     ///< Количество паттернов
    PatternStatistic patterns[MAX_PATTERNS]; ///< Статистика по каждому паттерну
};