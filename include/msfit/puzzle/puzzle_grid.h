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

#include "msfit/puzzle/grid_word.h"
#include "msfit/puzzle/square.h"

class PuzzleGrid : public Gtk::DrawingArea {

  public:
    PuzzleGrid();
    PuzzleGrid(size_t nRows, size_t nCols);
    ~PuzzleGrid() {}

    size_t nRows() const;
    size_t nCols() const;
    void setRows(size_t rows);
    void setCols(size_t cols);
    void setSize(size_t rows, size_t cols);

    void save();
    void load();

    bool isTwoTurnSymmetric() const;
    bool isOneTurnSymmetric() const;
    bool isMirroredUpDown() const;
    bool isMirroredLeftRight() const;

    // Rendering help
    bool isAcrossSelected() const;
    bool areSquareIndicesValid(const std::array<int, 2>& indices) const;
    std::array<int, 2> getSelectedSquare() const;
    void setSelectedSquare(const std::array<int, 2>& indices);
    void renderSelectedSquare();

    // Get the next white square in the across/down direction, possibly the start of the next word.
    std::array<int, 2> getNextLogicalSquare(const std::array<int, 2>& indices, bool isAcross) const;
    std::array<int, 2> getPreviousLogicalSquare(const std::array<int, 2>& indices, bool isAcross) const;
    // Get the next white square in the given direction.
    std::array<int, 2> getNextGeometricSquare(const std::array<int, 2>& indices, guint keyval);

  private:
    // A bunch of variables/functions in PuzzleGrid, GridWord, and Square depend on pointers to elements in <data>;
    // these pointers will remain valid as long as <data> is not resized. Whenever the grid is resized (i.e. setSize()
    // is called), all pointers in these objects should be reset in getWords().
    std::vector<std::vector<Square>> data; // row-major order

    // Rendering info. Variables set in the draw() function.
    size_t xStart, yStart;      // pixel location of the upper left corner of the grid
    size_t squareSize;          // side length of each square
    bool acrossSelected = true; // are we in across mode?

    // Based on the current pattern of white/black squares, determine the words of the puzzle (contiguous blocks of
    // white squares) and their numbers.
    std::vector<GridWord> acrossWords, downWords;
    void setSquareNeighbors();
    void getWords();

    // Functions for signal handlers
    void draw(const Cairo::RefPtr<Cairo::Context>& cr, int gridWidth, int gridHeight);
    void on_left_click(int n_press, double x, double y);
    void on_right_click(int n_press, double x, double y);
    bool on_key_press(guint keyval, guint keycode, Gdk::ModifierType state, const Glib::ustring& phase);
    std::array<int, 2> mapClickToSquareIndex(double x, double y); // map cursor click to index of square in the grid

    // Interactive state variables particular to the grid.
    std::array<int, 2> selectedSquare; // indices of the currently selected square in <data>
};

#include "msfit/puzzle/puzzle_grid.ipp"