#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <semaphore.h>
#include "shared_statistics.hpp"

/// Менеджер для работы с разделяемой памятью (используется для записи в общую статистику между процесами)
class StatisticManager
{
public:
    /// Конструктор
    explicit StatisticManager(const std::vector<std::string> &patterns);
    /// Деструктор
    ~StatisticManager();

    /// Обновить данные по статистике
    void update(const std::unordered_map<std::string, size_t> &matches);

    /// Произвести запись в FIFO с актуальной статистикой
    void serveStats();

private:
    SharedStatistic *stat = nullptr; ///< Общая статистика по проверке
    sem_t *sem = nullptr;            ///< Семафор для синхронизации доступа
};