#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

//===========================================================================
// Пошук із поверненням (backtracking)
//===========================================================================

bool backtracking(SecureBox& box, std::vector<std::pair<uint32_t, uint32_t>>& path, uint32_t xSize, uint32_t ySize)
{
    if (!box.isLocked()) return true; // Якщо коробка розблокована, рішення знайдено.

    for (uint32_t y = 0; y < ySize; ++y)
    {
        for (uint32_t x = 0; x < xSize; ++x)
        {
            box.toggle(x, y); // Застосовуємо toggle.
            path.emplace_back(x, y); // Додаємо крок до шляху.

            if (backtracking(box, path, xSize, ySize)) return true; // Рекурсивний виклик.

            box.toggle(x, y); // Скасовуємо toggle.
            path.pop_back(); // Видаляємо крок зі шляху.
        }
    }

    return false; // Якщо не вдалося знайти рішення.
}

//===========================================================================
// Жадібний алгоритм
//===========================================================================

bool greedy(SecureBox& box, uint32_t xSize, uint32_t ySize)
{
    while (box.isLocked())
    {
        auto state = box.getState();
        uint32_t maxLocked = 0;
        uint32_t bestX = 0, bestY = 0;

        // Знаходимо клітинку, яка впливає на максимальну кількість заблокованих клітинок.
        for (uint32_t y = 0; y < ySize; ++y)
        {
            for (uint32_t x = 0; x < xSize; ++x)
            {
                uint32_t affected = 0;

                // Рахуємо кількість заблокованих клітинок, на які впливає toggle(x, y).
                for (uint32_t i = 0; i < ySize; ++i)
                    if (state[i][x] != 0) ++affected;

                for (uint32_t j = 0; j < xSize; ++j)
                    if (state[y][j] != 0) ++affected;

                if (state[y][x] != 0) --affected; // Прибираємо подвійний облік центральної клітинки.

                if (affected > maxLocked)
                {
                    maxLocked = affected;
                    bestX = x;
                    bestY = y;
                }
            }
        }

        if (maxLocked == 0) return false; // Якщо немає заблокованих клітинок, рішення неможливе.

        box.toggle(bestX, bestY); // Застосовуємо toggle до найкращої клітинки.
    }

    return true; // Коробка розблокована.
}

//===========================================================================
// Перебір всіх комбінацій (лише для невеликих розмірів)
//===========================================================================

bool bruteForce(SecureBox& box, uint32_t xSize, uint32_t ySize)
{
    uint32_t totalCells = xSize * ySize;
    auto state = box.getState();

    for (uint32_t mask = 0; mask < (1 << totalCells); ++mask)
    {
        SecureBox tempBox = box; // Створюємо копію коробки.
        for (uint32_t i = 0; i < totalCells; ++i)
        {
            if (mask & (1 << i))
            {
                uint32_t x = i % xSize;
                uint32_t y = i / xSize;
                tempBox.toggle(x, y); // Застосовуємо toggle для поточної комбінації.
            }
        }

        if (!tempBox.isLocked()) return true; // Якщо коробка розблокована, рішення знайдено.
    }

    return false; // Якщо не вдалося знайти рішення.
}

//===========================================================================
// main для тестування методів
//===========================================================================

int main()
{
    uint32_t xSize = 3, ySize = 3; // Розміри коробки.
    SecureBox box(xSize, ySize);

    std::vector<std::pair<uint32_t, uint32_t>> path;

    std::cout << "Backtracking: " << (backtracking(box, path, xSize, ySize) ? "Успіх" : "Невдача") << std::endl;

    SecureBox box2(xSize, ySize);
    std::cout << "Greedy: " << (greedy(box2, xSize, ySize) ? "Успіх" : "Невдача") << std::endl;

    SecureBox box3(xSize, ySize);
    std::cout << "Brute Force: " << (bruteForce(box3, xSize, ySize) ? "Успіх" : "Невдача") << std::endl;

    return 0;
}