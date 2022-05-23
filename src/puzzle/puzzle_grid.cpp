#include <msfit/engine/puzzle_grid.h>

namespace puzzle {

// Constructors
PuzzleGrid::PuzzleGrid() { PuzzleGrid(1, 1); }

PuzzleGrid::PuzzleGrid(size_t nRows_, size_t nCols_) {

    setSize(nRows_, nCols_);

    // Set up puzzle grid.
    renderedGrid.set_row_spacing(0);
    renderedGrid.set_column_spacing(0);
    for (size_t i = 0; i < nRows(); i++) {
        for (size_t j = 0; j < nCols(); j++) {
            renderedGrid.attach(data[i][j], j, i);
        }
    }
}

inline size_t PuzzleGrid::nRows() const { return nRows; }
inline size_t PuzzleGrid::nCols() const { return nCols; }

void PuzzleGrid::setSize(size_t rows, size_t cols) {

    // std::vector<T>::resize() either truncates or appends extra elements; have the added elements be blank white
    // squares.
    Square whiteSquare(puzzle::WHITE, "");
    data.resize(rows);
    for (size_t i = 0; i < rows; i++) {
        data[i].resize(cols, whiteSquare);
    }
    nRows = rows;
    nCols = cols;
}

void PuzzleGrid::draw(int width, int height) {

    cr->scale(width, height);
    int squareSize = std::min(width / nCols(), height / nRows());

    // Place all the Gtk::DrawingAreas of the squares in a grid. Each Square manages its own signals.
}

} // namespace puzzle