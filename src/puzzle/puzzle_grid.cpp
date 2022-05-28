#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gestureclick.h>

#include "msfit/puzzle/puzzle_grid.h"
#include "msfit/utilities/state.h"

// Constructors
PuzzleGrid::PuzzleGrid() { PuzzleGrid(grid::params::N_ROWS, grid::params::N_COLS); }

PuzzleGrid::PuzzleGrid(size_t nRows_, size_t nCols_) {
    setSize(nRows_, nCols_);
    getWords();
    renderedGrid.set_draw_func(sigc::mem_fun(*this, &PuzzleGrid::draw));

    // Set up non-default event handlers for Gtk::DrawingArea.
    renderedGrid.set_focus_on_click(true); // DrawingArea will be "focused" when clicked
    renderedGrid.set_can_target(true);     // DrawingArea can be the target of pointer events

    // gtkmm4 moved all signal-handling functionality to "Gesture*" objects (clicks) and EventController
    // objects (keys.)
    auto leftClickHandler = Gtk::GestureClick::create();
    leftClickHandler->set_button(GDK_BUTTON_PRIMARY); // the left mouse button
    leftClickHandler->signal_pressed().connect(sigc::mem_fun(*this, &PuzzleGrid::on_left_click));
    renderedGrid.add_controller(leftClickHandler);

    auto rightClickHandler = Gtk::GestureClick::create();
    rightClickHandler->set_button(GDK_BUTTON_SECONDARY); // the right mouse button
    rightClickHandler->signal_pressed().connect(sigc::mem_fun(*this, &PuzzleGrid::on_right_click));
    renderedGrid.add_controller(rightClickHandler);


    // auto keyHandler = Gtk::EventControllerKey::create();
    // keyHandler->signal_key_pressed().connect(sigc::mem_fun(*this, &ExampleWindow::on_key_pressed), false);
    // add_controller(keyHandler);
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

void PuzzleGrid::setRows(size_t rows) {
    if (rows < 1) {
        std::cerr << "Number of rows cannot be less than 1." << std::endl;
        return;
    }
    Square whiteSquare(false, "");
    data.resize(rows);
}
void PuzzleGrid::setCols(size_t cols) {
    if (cols < 1) {
        std::cerr << "Number of cols cannot be less than 1." << std::endl;
        return;
    }
    Square whiteSquare(false, "");
    for (size_t i = 0; i < nRows(); i++) {
        data[i].resize(cols, whiteSquare);
    }
}

void PuzzleGrid::draw(const Cairo::RefPtr<Cairo::Context>& cr, int gridWidth, int gridHeight) {

    squareSize = std::min((gridWidth - 2) / nCols(), (gridHeight - 2) / nRows());
    // The starting point of upper left corner of the grid in the canvas -- try to center the grid.
    size_t x, y;
    size_t puzzleWidth = squareSize * nCols();
    size_t puzzleHeight = squareSize * nRows();
    x = (gridWidth - puzzleWidth) / 2;
    y = (gridHeight - puzzleHeight) / 2;
    xStart = x;
    yStart = y;

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
            bool isNumberSet = false;
            // Start of a new across word
            if (j == 0 || data[i][j - 1].isSolid()) {
                data[i][j].setNumber(currNum);
                isNumberSet = true;
                acrossWords.push_back(std::vector<Square*>());
                nAcross++;
                currNum++;
            }
            data[i][j].setAcrossWord(nAcross - 1);
            acrossWords[nAcross - 1].push_back(&data[i][j]);
            // Start of a new down word
            if (i == 0 || data[i - 1][j].isSolid()) {
                if (!isNumberSet) {
                    data[i][j].setNumber(currNum);
                    currNum++;
                }
                downWords.push_back(std::vector<Square*>());
                nDown++;
            }
            data[i][j].setDownWord(nDown - 1);
            downWords[nDown - 1].push_back(&data[i][j]);
        }
    }
}

// =================================== SYMMETRY CHECKERS ===================================

bool PuzzleGrid::isTwoTurnSymmetric() const {
    size_t rows = nRows();
    size_t cols = nCols();
    if (rows != cols) return false;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (data[i][j].isSolid() != data[rows - i - 1][cols - j - 1].isSolid()) return false;
        }
    }
    return true;
}

bool PuzzleGrid::isOneTurnSymmetric() const {
    size_t rows = nRows();
    size_t cols = nCols();
    if (rows != cols) return false;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (data[i][j].isSolid() != data[i][cols - j - 1].isSolid()) return false;
        }
    }
    return true;
}

bool PuzzleGrid::isMirroredUpDown() const {
    size_t rows = nRows();
    size_t cols = nCols();
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (data[i][j].isSolid() != data[rows - i - 1][j].isSolid()) return false;
        }
    }
    return true;
}

bool PuzzleGrid::isMirroredLeftRight() const {
    size_t rows = nRows();
    size_t cols = nCols();
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (data[i][j].isSolid() != data[i][cols - j - 1].isSolid()) return false;
        }
    }
    return true;
}

// =================================== SIGNAL HANDLERS ===================================

void PuzzleGrid::on_left_click(int n_press, double x, double y) {

    // Determine which square was clicked.
    std::array<int, 2> indices = mapClickToSquareIndex(x, y);
    // If outside the grid, don't do anything.
    int ind_i = indices[0];
    int ind_j = indices[1];
    if (ind_i < 0 || ind_j < 0 || ind_i >= nRows() || ind_j >= nCols()) return;

    setSelectedSquare(indices);

    renderSelectedSquare();

    // Request redraw.
    renderedGrid.queue_draw();
}

void PuzzleGrid::on_right_click(int n_press, double x, double y) {

    // TODO: Toggle selected square white/black; re-number words; if "Make symmetric" is toggled, make sure that the
    // puzzle remains symmetric.

    // Request redraw.
    renderedGrid.queue_draw();
}

/*
 * Based on which square is selected, render the square appropriately, along with the word it's contained in.
 */
void PuzzleGrid::renderSelectedSquare() {

    int ind_i = selectedSquare[0];
    int ind_j = selectedSquare[1];
    if (ind_i < 0 || ind_j < 0 || ind_i >= nRows() || ind_j >= nCols()) return;

    // De-select all squares first.
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j].setSelectionStatus(cell::state::UNSELECTED);
        }
    }
    // Mark the squares in the current word (either across or down.)
    if (isAcrossSelected()) {
        size_t word = data[ind_i][ind_j].getAcrossWord();
        for (auto sq : acrossWords[word]) {
            sq->setSelectionStatus(cell::state::HIGHLIGHTED);
        }
    } else {
        size_t word = data[ind_i][ind_j].getDownWord();
        for (auto sq : downWords[word]) {
            sq->setSelectionStatus(cell::state::HIGHLIGHTED);
        }
    }
    // Mark this square as selected.
    data[ind_i][ind_j].setSelectionStatus(cell::state::SELECTED);
}

/*
 * Given the pixel locations of a click, determine which square was clicked.
 *
 * WARNING: The mapping only works if the initial window size is not larger than what can fit on the current device
 * screen. Otherwise, some implicit resizing goes on behind the scenes in a way that I don't quite understand yet, and
 * this mapping only works properly after window gets resized.
 */
std::array<int, 2> PuzzleGrid::mapClickToSquareIndex(double x, double y) {

    // Negative numbers get rounded up, positive numbers get rounded down; just return -1 or a number greater than the
    // max index if click is outside the grid.
    int i = (y - yStart) < 0 ? -1 : std::floor((y - yStart) / squareSize);
    int j = (x - xStart) < 0 ? -1 : std::floor((x - xStart) / squareSize);
    return {i, j};
}

// If not a supported key, do nothing and return.
// Right-click: Toggle squares black/white.
// If "Make symmetric" is toggled, mirror selected black/white squares appropriately.
// `Space`: Toggle across/down.
// `Tab`: Skip to the next uncompleted word.
// `Esc`: Enter more than one letter in a box. Hit Esc again or `Enter' to exit this mode.
// `Enter': Search fills according to user-defined criteria. Generate and display a list of top 10 fills.
// Arrow keys: move one box in the corrresponding direction (skipping black squares.) This moves in the direction you'd
// expect in the grid, not necessarily to the next word, in order.
// Any letter key: Enter the capital letter in the currently
// selected box (if any); render in gray if the pencil icon is selected.
//
// +/- new column: complete re-number (getWords())
// +/- new row: complete re-number (getWords())
// resize: complete re-number (getWords())
//
// redraw after all operations