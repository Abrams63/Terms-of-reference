# Function Documentation

## Class `SecureBox`

### Constructor `SecureBox(uint32_t x, uint32_t y)`
Creates an object of the `SecureBox` class with specified dimensions `x` and `y`.

- **Parameters:**
  - `x` (uint32_t): width of the box.
  - `y` (uint32_t): height of the box.

- **Description:**
  Initializes the box with the specified dimensions, fills it randomly, and shuffles its state.

---

### `void toggle(uint32_t x, uint32_t y)`
Toggles the state of cells in the specified row and column.

- **Parameters:**
  - `x` (uint32_t): column index.
  - `y` (uint32_t): row index.

- **Description:**
  Changes the state of cells in the specified row and column, compensating for the change in the central cell.

---

### `bool isLocked()`
Checks if the box is locked.

- **Returns:**
  - `true` if at least one cell is not equal to 0.
  - `false` if all cells are equal to 0.

---

### `std::vector<std::vector<uint8_t>> getState()`
Returns the current state of the box.

- **Returns:**
  - A two-dimensional vector representing the state of the box.

---

### `void shuffle()`
Randomly shuffles the state of the box.

---

## Function `bool solveBox(std::vector<std::vector<uint8_t>>& state, uint32_t xSize, uint32_t ySize, std::vector<std::vector<uint8_t>>& presses)`

Solves the system of equations to unlock the box.

- **Parameters:**
  - `state` (std::vector<std::vector<uint8_t>>): current state of the box.
  - `xSize` (uint32_t): width of the box.
  - `ySize` (uint32_t): height of the box.
  - `presses` (std::vector<std::vector<uint8_t>>&): matrix of presses to unlock the box.

- **Returns:**
  - `true` if a solution is found.
  - `false` if no solution is found.

- **Description:**
  Uses the Gaussian method to solve the system of equations modulo 3 to determine the sequence of presses to unlock the box.

---

## Function `bool openBox(SecureBox& box)`

Attempts to unlock the box.

- **Parameters:**
  - `box` (SecureBox&): box object.

- **Returns:**
  - `true` if the box is successfully unlocked.
  - `false` if the box remains locked.

- **Description:**
  Retrieves the current state of the box, calculates the sequence of presses to unlock it, and applies them.

---

## Function `int main(int argc, char* argv[])`

Entry point of the program.

- **Parameters:**
  - `argc` (int): number of command-line arguments.
  - `argv` (char*[]): array of command-line arguments.

- **Returns:**
  - `0` if the box is successfully unlocked.
  - `1` if the box remains locked.

- **Description:**
  Creates a `SecureBox` object with dimensions specified in the command-line arguments and attempts to unlock it. Outputs the result to the console.