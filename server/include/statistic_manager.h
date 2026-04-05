#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <semaphore.h>
#include <shared_statistics.hpp>

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