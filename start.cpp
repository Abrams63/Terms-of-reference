#include <iostream>
#include <vector>
#include <random>
#include <time.h>

//===========================================================================
// # PROBLEM: Total Unlocking of the SecureBox
//===========================================================================
// SecureBox is a 2D grid (y rows × x columns) of integer values:
//     0 → fully unlocked
//     1 → partially locked
//     2 → fully locked
//
// Goal: Use toggle(x, y) operations to reach a fully unlocked state (all 0s).
// 
//===========================================================================
// # SecureBox API
//===========================================================================
//
// Public methods of the SecureBox class:
//
// void toggle(uint32_t x, uint32_t y)
//     Increments the state of:
//         - the cell at (x, y)
//         - all cells in the same row
//         - all cells in the same column
//
// bool isLocked()
//     Returns true if at least one cell is non-zero (locked or partially locked).
//     Returns false only when all cells are 0 (unlocked).
//
// std::vector<std::vector<uint8_t>> getState()
//     Returns a copy of the current box state (2D grid of values).
// 
//================================================================================

//================================================================================
// Class: SecureBox
// Description:
//     Represents a 2D grid of integer states:
//         0 = fully unlocked
//         1 = partially locked
//         2 = fully locked
//================================================================================
class SecureBox
{
private:
    std::vector<std::vector<uint8_t>> box;
    std::mt19937_64 rng;
    uint32_t xSize, ySize;

public:

    //================================================================================
    // Constructor: SecureBox
    // Description:
    //     Initializes the box with dimensions x × y and randomizes the grid
    //     using pseudo-random toggle operations.
    //================================================================================
    SecureBox(uint32_t x, uint32_t y) : xSize(x), ySize(y)
    {
        rng.seed(time(0));
        box.resize(y);
        for (auto& row : box)
            row.resize(x, false);
        shuffle();
    }

    //================================================================================
    // Method: toggle
    // Description:
    //     Applies modulo-3 increment to:
    //         - all cells in column x (↑↓)
    //         - all cells in row y (←→)
    //         - compensates the (x, y) cell by incrementing it again (+2 mod 3)
    //================================================================================
    void toggle(uint32_t x, uint32_t y)
    {
        // Vertical (column)
        for (uint32_t row = 0; row < ySize; ++row)
            box[row][x] = (box[row][x] + 1) % 3;

        // Horizontal (row)
        for (uint32_t col = 0; col < xSize; ++col)
            box[y][col] = (box[y][col] + 1) % 3;

        // Center cell was incremented twice, fix it to be +1 total
        box[y][x] = (box[y][x] + 2) % 3;
    }

    //================================================================================
    // Method: isLocked
    // Description:
    //     Returns true if any cell is not 0 (i.e. locked or partially locked).
    //     Returns false only if all cells are fully unlocked (0).
    //================================================================================
    bool isLocked()
    {
        for (const auto& row : box)
            for (bool cell : row)
                if (cell)
                    return true;
        return false;
    }

    //================================================================================
    // Method: getState
    // Description:
    //     Returns a deep copy of the current grid state.
    //================================================================================
    std::vector<std::vector<uint8_t>> getState()
    {
        return box;
    }

private:

    //================================================================================
    // Method: shuffle
    // Description:
    //     Randomly toggles cells multiple times to generate
    //     a scrambled starting configuration.
    //================================================================================
    void shuffle()
    {
        for (uint32_t t = rng() % 0x1000; t > 0; --t)
            toggle(rng() % xSize, rng() % ySize);
    }
};

//================================================================================
// Function: openBox
// Description:
//     Placeholder for unlocking algorithm. Currently does nothing
//     and simply returns the current state.
//================================================================================
bool openBox(SecureBox& box)
{
    // TODO: Implement actual unlocking algorithm using toggle().
    // ...

    return !box.isLocked();
}

int main(int argc, char* argv[])
{
    uint32_t x = std::atol(argv[1]);
    uint32_t y = std::atol(argv[2]);
    SecureBox box(x, y);
    bool state = openBox(box);

    if (!state)
        std::cout << "BOX: LOCKED!" << std::endl;
    else
        std::cout << "BOX: OPENED!" << std::endl;

    return state;
}