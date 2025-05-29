#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>

//===========================================================================
// SecureBox class
//===========================================================================

class SecureBox
{
private:
    std::vector<std::vector<uint8_t>> box;
    std::mt19937_64 rng;
    uint32_t xSize, ySize;

public:
    SecureBox(uint32_t x, uint32_t y) : xSize(x), ySize(y)
    {
        rng.seed(time(0));
        box.resize(y);
        for (auto& row : box)
            row.resize(x, false);
        shuffle();
    }

    void toggle(uint32_t x, uint32_t y)
    {
        // Column
        for (uint32_t row = 0; row < ySize; ++row)
            box[row][x] = (box[row][x] + 1) % 3;

        // Row
        for (uint32_t col = 0; col < xSize; ++col)
            box[y][col] = (box[y][col] + 1) % 3;

        // Compensate center
        box[y][x] = (box[y][x] + 2) % 3;
    }

    bool isLocked()
    {
        for (const auto& row : box)
            for (uint8_t cell : row)
                if (cell != 0)
                    return true;
        return false;
    }

    std::vector<std::vector<uint8_t>> getState()
    {
        return box;
    }

private:
    void shuffle()
    {
        for (uint32_t t = rng() % 0x1000; t > 0; --t)
            toggle(rng() % xSize, rng() % ySize);
    }
};

//===========================================================================
// Gaussian Elimination mod 3
//===========================================================================

bool solveBox(std::vector<std::vector<uint8_t>>& state, uint32_t xSize, uint32_t ySize, std::vector<std::vector<uint8_t>>& presses)
{
    const uint32_t N = xSize * ySize;
    std::vector<std::vector<uint8_t>> A(N, std::vector<uint8_t>(N + 1, 0)); // Ax = b

    // Build system
    for (uint32_t y = 0; y < ySize; ++y)
    {
        for (uint32_t x = 0; x < xSize; ++x)
        {
            uint32_t idx = y * xSize + x;

            // Column
            for (uint32_t i = 0; i < ySize; ++i)
                A[i * xSize + x][idx] = (A[i * xSize + x][idx] + 1) % 3;

            // Row
            for (uint32_t j = 0; j < xSize; ++j)
                A[y * xSize + j][idx] = (A[y * xSize + j][idx] + 1) % 3;

            // Center cell adjustment
            A[idx][idx] = (A[idx][idx] + 1) % 3;
        }
    }

    // Right-hand side (target is zero → 3 - current mod 3)
    for (uint32_t y = 0; y < ySize; ++y)
        for (uint32_t x = 0; x < xSize; ++x)
            A[y * xSize + x][N] = (3 - state[y][x]) % 3;

    // Gaussian elimination
    for (uint32_t col = 0, row = 0; col < N && row < N; ++col)
    {
        int pivot = -1;
        for (uint32_t i = row; i < N; ++i)
        {
            if (A[i][col] != 0) { pivot = i; break; }
        }

        if (pivot == -1) continue;

        std::swap(A[row], A[pivot]);

        uint8_t inv = (A[row][col] == 1) ? 1 : 2; // inverse of 1 is 1, inverse of 2 is 2 mod 3
        for (uint32_t j = 0; j <= N; ++j)
            A[row][j] = (A[row][j] * inv) % 3;

        for (uint32_t i = 0; i < N; ++i)
        {
            if (i == row) continue;
            uint8_t factor = A[i][col];
            for (uint32_t j = 0; j <= N; ++j)
                A[i][j] = (A[i][j] + 3 - factor * A[row][j] % 3) % 3;
        }

        ++row;
    }

    // Extract solution
    std::vector<uint8_t> solution(N, 0);
    for (uint32_t i = 0; i < N; ++i)
        solution[i] = A[i][N];

    presses.resize(ySize, std::vector<uint8_t>(xSize, 0));
    for (uint32_t i = 0; i < N; ++i)
    {
        uint32_t y = i / xSize;
        uint32_t x = i % xSize;
        presses[y][x] = solution[i];
    }

    return true;
}

//===========================================================================
// openBox function
//===========================================================================

bool openBox(SecureBox& box)
{
    auto state = box.getState();
    uint32_t ySize = state.size();
    uint32_t xSize = state[0].size();

    std::vector<std::vector<uint8_t>> pressMatrix;
    if (!solveBox(state, xSize, ySize, pressMatrix))
        return false;

    for (uint32_t y = 0; y < ySize; ++y)
        for (uint32_t x = 0; x < xSize; ++x)
            for (uint8_t i = 0; i < pressMatrix[y][x]; ++i)
                box.toggle(x, y);

    return !box.isLocked();
}

//===========================================================================
// main
//===========================================================================

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./securebox <width> <height>\n";
        return 1;
    }

    uint32_t x = std::atoi(argv[1]);
    uint32_t y = std::atoi(argv[2]);

    SecureBox box(x, y);

    bool unlocked = openBox(box);
    if (unlocked)
        std::cout << "BOX: OPENED!" << std::endl;
    else
        std::cout << "BOX: LOCKED!" << std::endl;

    return unlocked ? 0 : 1;
}
