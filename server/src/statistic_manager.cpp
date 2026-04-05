#include "statistic_manager.h"

#include <stdexcept>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

namespace
{
    const char *SEM_NAME = "/server_sem";
}

StatisticManager::StatisticManager(const std::vector<std::string> &patterns)
{
    if (patterns.size() > MAX_PATTERNS)
    {
        throw std::runtime_error("Превышено кол-во паттернов. Максимальное кол-во: " + std::to_string(MAX_PATTERNS));
    }

    stat = static_cast<SharedStatistic *>(mmap(
        nullptr,
        sizeof(SharedStatistic),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0));

    if (stat == MAP_FAILED)
    {
        throw std::runtime_error("Не удалось создать разделяемую память");
    }

    memset(stat, 0, sizeof(SharedStatistic));

    stat->patternCount = static_cast<size_t>(patterns.size());
    for (size_t i = 0; i < stat->patternCount; i++)
    {
        strncpy(stat->patterns[i].name, patterns[i].c_str(), MAX_PATTERN_LEN - 1);
        stat->patterns[i].name[MAX_PATTERN_LEN - 1] = '\0';
        stat->patterns[i].matchCount = 0;
    }

    sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED)
    {
        munmap(stat, sizeof(SharedStatistic));
        throw std::runtime_error("Не удалось создать семафор");
    }
}

StatisticManager::~StatisticManager()
{
    if (stat != MAP_FAILED && stat != nullptr)
    {
        munmap(stat, sizeof(SharedStatistic));
        stat = nullptr;
    }

    if (sem != SEM_FAILED && sem != nullptr)
    {
        sem_close(sem);
        sem_unlink(SEM_NAME);
        sem = nullptr;
    }
}

void StatisticManager::update(const std::unordered_map<std::string, size_t> &matches)
{
    sem_wait(sem);

    stat->checkedFilesCount++;

    for (size_t i = 0; i < stat->patternCount; i++)
    {
        auto it = matches.find(stat->patterns[i].name);
        if (it != matches.end())
        {
            stat->patterns[i].matchCount += it->second;
        }
    }

    sem_post(sem);
}

void StatisticManager::serveStats()
{
    int fifoFd = open(FIFO_PATH, O_WRONLY);
    if (fifoFd < 0)
    {
        return;
    }

    sem_wait(sem);
    write(fifoFd, stat, sizeof(SharedStatistic));
    sem_post(sem);

    close(fifoFd);
}