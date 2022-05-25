#include "msfit/puzzle/puzzle_grid.h"
#include "msfit/utilities/state.h"

// Constructors
PuzzleGrid::PuzzleGrid() { PuzzleGrid(state::N_ROWS, state::N_COLS); }

PuzzleGrid::PuzzleGrid(size_t nRows_, size_t nCols_) {
    setSize(nRows_, nCols_);
    getWords();
    renderedGrid.set_draw_func(sigc::mem_fun(*this, &PuzzleGrid::draw));
}

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

    size_t squareSize = std::min((gridWidth - 2) / nCols(), (gridHeight - 2) / nRows());
    // The starting point of upper left corner of the grid in the canvas -- try to center the grid.
    size_t x, y;
    size_t puzzleWidth = squareSize * nCols();
    size_t puzzleHeight = squareSize * nRows();
    x = (gridWidth - puzzleWidth) / 2;
    y = (gridHeight - puzzleHeight) / 2;

    // Draw a black border around the whole grid by first drawing a black rectangle spanning the whole canvas.
    size_t border = theme::puzzle_border_width;
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->rectangle(x - 0.5 * border, y - 0.5 * border, puzzleWidth + border, puzzleHeight + border);
    cr->fill();

    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j].draw(renderedGrid, cr, squareSize, x + squareSize * j, y + squareSize * i);
        }
    }
}

/*
 * Get *all* the words in the puzzle from scratch, and assign numbers to squares (if they are at the start of a word)
 * along the way.
 */
void PuzzleGrid::getWords() {
    acrossWords.clear();
    downWords.clear();
    // A square is the start of a new across/down word iff it doesn't have a (white) neighbor to the left/above.
    size_t nAcross = 0;
    size_t nDown = 0;
    int currNum = 1;
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            if (data[i][j].isSolid()) {
                data[i][j].setNumber(-1);
                continue;
            }
            // Start of a new across word
            if (j == 0 || data[i][j - 1].isSolid()) {
                data[i][j].setNumber(currNum);
                acrossWords.push_back(std::vector<Square*>());
                nAcross++;
                currNum++;
            }
            data[i][j].setAcrossWord(nAcross - 1);
            acrossWords[nAcross - 1].push_back(&data[i][j]);
            // Start of a new down word
            if (i == 0 || data[i - 1][j].isSolid()) {
                data[i][j].setNumber(currNum);
                downWords.push_back(std::vector<Square*>());
                nDown++;
                currNum++;
            }
            data[i][j].setDownWord(nDown - 1);
            downWords[nDown - 1].push_back(&data[i][j]);
        }
    }
}

// /*
//  * Assumed that x and y are normalized to be between 0 and 1.
//  */
// std::array<size_t, 2> PuzzleGrid::mapClickToSquare(int x, int y) {

//     double 1.0/nRows();
//     double 1.0/nCols();
// }