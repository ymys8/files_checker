#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <semaphore.h>

const int MAX_PATTERNS = 64;
const int MAX_PATTERN_LEN = 128;

///
struct PatternStatistic
{
    char name[MAX_PATTERN_LEN]; ///<
    size_t matchCount;          ///<
};

///
struct SharedStatistic
{
    size_t checkedFilesCount;                ///<
    size_t patternCount;                     ///<
    PatternStatistic patterns[MAX_PATTERNS]; ///<
};

///
class StatisticManager
{
public:
    ///
    explicit StatisticManager(const std::vector<std::string> &patterns);
    ///
    ~StatisticManager();

    ///
    void update(const std::unordered_map<std::string, size_t> &matches);

private:
    SharedStatistic *stat = nullptr; ///<
    sem_t *sem = nullptr;            ///<
};