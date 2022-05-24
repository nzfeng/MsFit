#include "msfit/puzzle/puzzle_grid.h"
#include "msfit/utilities/state.h"

// Constructors
PuzzleGrid::PuzzleGrid() { PuzzleGrid(state::N_ROWS, state::N_COLS); }

PuzzleGrid::PuzzleGrid(size_t nRows_, size_t nCols_) { setSize(nRows_, nCols_); }

inline size_t PuzzleGrid::nRows() const { return data.size(); }
inline size_t PuzzleGrid::nCols() const { return data[0].size(); }

void PuzzleGrid::setSize(size_t rows, size_t cols) {

    if ((rows < 1) || (cols < 1)) {
        std::cerr << "Number of rows/cols cannot be less than 1." << std::endl;
        return;
    }
    // std::vector<T>::resize() either truncates or appends extra elements; have the added elements be blank white
    // squares.
    Square whiteSquare(false, "");
    data.resize(rows);
    for (size_t i = 0; i < rows; i++) {
        data[i].resize(cols, whiteSquare);
    }
}

void PuzzleGrid::draw(const Cairo::RefPtr<Cairo::Context>& cr, int gridWidth, int gridHeight) {

    size_t squareSize = std::min(gridWidth / nCols(), gridHeight / nRows());
    size_t x, y; // starting point of upper left corner of the grid in the canvas
    x = (gridWidth - squareSize * nCols()) / 2;
    y = (gridHeight - squareSize * nRows()) / 2;

    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j].draw(renderedGrid, cr, squareSize, x + squareSize * i, y + squareSize * j);
        }
    }
}