/*
 * A PuzzleGrid structure is used when the puzzle only consists of square cells (the standard and common case.)
 * We only need a simple 2D array data structure, and no fancy navigation methods that will add unnecessary overhead.
 */

#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>
#include <msfit/engine/square.h>

namespace puzzle {

class PuzzleGrid {
  public:
    PuzzleGrid();
    PuzzleGrid(size_t nRows, size_t nCols);
    ~PuzzleGrid();

    size_t nRows() const;
    size_t nCols() const;
    void setSize(size_t rows, size_t cols);

    void save();
    void load();

  private:
    std::vector<std::vector<Square>> data; // row-major order
    Gtk::Grid renderedGrid;

    // Based on the current pattern of white/black squares, determine the words of the puzzle (contiguous blocks of
    // white squares) and their numbers.
    void determineWords();
};
} // namespace puzzle