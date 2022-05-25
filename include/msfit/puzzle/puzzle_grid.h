/*
 * A PuzzleGrid structure is used when the puzzle only consists of square cells (the standard and common case.)
 * We only need a simple 2D array data structure, and no fancy navigation methods that will add unnecessary overhead.
 */

#pragma once

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>

#include "msfit/puzzle/square.h"

class PuzzleGrid {
  public:
    PuzzleGrid();
    PuzzleGrid(size_t nRows, size_t nCols);
    ~PuzzleGrid() {}

    size_t nRows() const;
    size_t nCols() const;
    void setSize(size_t rows, size_t cols);

    void save();
    void load();

    bool isAcrossSelected() const;

    Gtk::DrawingArea renderedGrid;

  private:
    std::vector<std::vector<Square>> data; // row-major order

    // Rendering info. Variables set in the draw() function.
    size_t xStart, yStart;      // pixel location of the upper left corner of the grid
    size_t squareSize;          // side length of each square
    bool acrossSelected = true; // are we in across mode?

    // Based on the current pattern of white/black squares, determine the words of the puzzle (contiguous blocks of
    // white squares) and their numbers.
    std::vector<std::vector<Square*>> acrossWords;
    std::vector<std::vector<Square*>> downWords;
    void getWords();

    // Functions for signal handlers
    void draw(const Cairo::RefPtr<Cairo::Context>& cr, int gridWidth, int gridHeight);
    void on_click(int n_press, double x, double y);
    std::array<int, 2> mapClickToSquareIndex(double x, double y); // map cursor click to index of square in the grid
};

#include "msfit/puzzle/puzzle_grid.ipp"