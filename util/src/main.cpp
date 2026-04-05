#include "shared_statistics.hpp"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        std::cerr << "Ошибка: сервер не запущен\n";
        return 1;
    }

    SharedStatistic stats{};
    std::size_t totalRead = 0;
    while (totalRead < sizeof(SharedStatistic))
    {
        ssize_t n = read(fd, reinterpret_cast<char *>(&stats) + totalRead,
                           sizeof(SharedStatistic) - totalRead);
        if (n < 0)
        {
            std::cerr << "Не удалось корректно считать статистику\n";
            close(fd);
            return 1;
        }
        if (n == 0)
            break;
        totalRead += n;
    }
    close(fd);

    std::cout << "Файлов проанализировано: " << stats.checkedFilesCount << '\n';
    std::cout << "Обнаруженные паттерны:\n";
    for (size_t i = 0; i < stats.patternCount; i++)
    {
        std::cout << stats.patterns[i].name
                  << ": " << stats.patterns[i].matchCount << '\n';
    }

    return 0;
}