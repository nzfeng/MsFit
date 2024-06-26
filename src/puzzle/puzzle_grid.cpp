#include <gtkmm/accelerator.h>
#include <gtkmm/entry.h>
#include <gtkmm/eventcontrollerfocus.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gestureclick.h>

#include "msfit/puzzle/puzzle_grid.h"
#include "msfit/utilities/utils.h"

// Constructors
PuzzleGrid::PuzzleGrid() { PuzzleGrid(grid::params::initRows, grid::params::initCols); }

PuzzleGrid::~PuzzleGrid() {}

PuzzleGrid::PuzzleGrid(size_t nRows_, size_t nCols_) {
    setSize(nRows_, nCols_);
    set_draw_func(sigc::mem_fun(*this, &PuzzleGrid::draw));

    // Set up non-default event handlers for Gtk::DrawingArea.
    set_can_focus(true); // whether the input focus can enter the widget or any of its children
    set_focusable(true); // Specifies whether widget can own the input focus; this seems to always keep focus on grid
    set_focus_on_click(true); // DrawingArea will be "focused" when clicked (this doesn't seem to work??)
    set_can_target(true);     // DrawingArea can be the target of pointer events

    // gtkmm4 moved all signal-handling functionality to "Gesture*" objects (clicks) and EventController
    // objects (keys.)
    auto leftClickHandler = Gtk::GestureClick::create();
    leftClickHandler->set_button(GDK_BUTTON_PRIMARY); // the left mouse button
    leftClickHandler->signal_pressed().connect(sigc::mem_fun(*this, &PuzzleGrid::on_left_click));
    add_controller(leftClickHandler);

    auto rightClickHandler = Gtk::GestureClick::create();
    rightClickHandler->set_button(GDK_BUTTON_SECONDARY); // the right mouse button
    rightClickHandler->signal_pressed().connect(sigc::mem_fun(*this, &PuzzleGrid::on_right_click));
    add_controller(rightClickHandler);

    auto keyHandler = Gtk::EventControllerKey::create();
    keyHandler->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "bubble"),
                                             false);
    add_controller(keyHandler);

    // keyHandler = Gtk::EventControllerKey::create();
    // keyHandler->set_propagation_phase(Gtk::PropagationPhase::TARGET);
    // keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "target"),
    //                                          false);
    // add_controller(keyHandler);

    // keyHandler->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    // keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "capture"),
    //                                          false);
    // add_controller(keyHandler);

    auto focusHandler = Gtk::EventControllerFocus::create();
    focusHandler->signal_leave().connect(sigc::mem_fun(*this, &PuzzleGrid::on_focus_out));
    add_controller(focusHandler);
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
    // Always re-compute cell neighbors.
    setSquareNeighbors();

    // Always re-compute words & reset pointers to squares in Square, GridWord objects.
    determineWords();

    queue_draw();
}

void PuzzleGrid::setRows(size_t rows) { setSize(rows, nCols()); }
void PuzzleGrid::setCols(size_t cols) { setSize(nRows(), cols); }

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

    renderSelectedSquare();

    // Draw a black border around the whole grid by first drawing a black rectangle spanning the whole canvas.
    size_t border = theme::puzzle_border_width;
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->rectangle(x - 0.5 * border, y - 0.5 * border, puzzleWidth + border, puzzleHeight + border);
    cr->fill();

    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            // Pass upper left corner coordinates of square
            data[i][j].draw(*this, cr, squareSize, x + squareSize * j, y + squareSize * i);
        }
    }

    // Highlight a word, if any.
    if (hlWordLifetime == 0) renderHighlightedWord(cr, squareSize);
}

/*
 * Highlight a word by drawing an outline around the whole word.
 */
void PuzzleGrid::renderHighlightedWord(const Cairo::RefPtr<Cairo::Context>& cr, const size_t& squareSize) {

    if (hlWord == NULL) return;

    // Determine the 4 corners of the rectangle that the word inhabits.
    size_t n = hlWord->length();
    std::array<size_t, 2> c1, c2, c3, c4;
    auto [firstSq_i, firstSq_j] = hlWord->squares[0]->getPosition();
    auto [lastSq_i, lastSq_j] = hlWord->squares[n - 1]->getPosition();
    c2 = {xStart + squareSize * firstSq_j, yStart + squareSize * firstSq_i};           // firstSq::upperLeft
    c4 = {xStart + squareSize * (lastSq_j + 1), yStart + squareSize * (lastSq_i + 1)}; // lastSq::lowerRight
    if (hlWord->isAcross) {
        // firstSq::lowerLeft -> firstSq::upperLeft -> lastSq::upperRight -> lastSq::lowerRight
        c1 = {xStart + squareSize * firstSq_j, yStart + squareSize * (firstSq_i + 1)}; // firstSq::lowerLeft
        c3 = {xStart + squareSize * (lastSq_j + 1), yStart + squareSize * lastSq_i};   // lastSq::upperRight
    } else {
        // firstSq::upperRight -> firstSq::upperLeft -> lastSq::lowerLeft -> lastSq::lowerRight
        c1 = {xStart + squareSize * (firstSq_j + 1), yStart + squareSize * firstSq_i}; // firstSq::upperRight
        c3 = {xStart + squareSize * lastSq_j, yStart + squareSize * (lastSq_i + 1)};   // lastSq::lowerLeft
    }

    std::array<float, 3> color = theme::color_word_highlight;
    cr->set_source_rgb(color[0], color[1], color[2]);
    // cr->set_line_join(Cairo::LineJoin::MITER); // Miter, Bevel, Round
    // cr->set_line_cap(); // Butt, Round, Square
    cr->move_to(c1[0], c1[1]);
    cr->line_to(c2[0], c2[1]);
    cr->line_to(c3[0], c3[1]);
    cr->line_to(c4[0], c4[1]);
    cr->line_to(c1[0], c1[1]);
    cr->stroke();

    hlWordLifetime++;
}

/*
 * Set a word to be highlighted. Normally a "rubber-banding" technique would be optimal to fulfill this function, but it
 * seems that a Cairo::Context can't be accessed outside of the drawing callback.
 *
 * The highlighting is dropped when the user does something that results in the grid being redrawn (i.e. enters a
 * letter, adjusts the grid settings.)
 */
void PuzzleGrid::setHighlightedWord(GridWord* word) {

    hlWordLifetime = 0;
    hlWord = word;
    queue_draw();
}

/*
 * Set information within each Square object letting it know how it is contained within the PuzzleGrid.
 */
void PuzzleGrid::setSquareNeighbors() {
    // Update the neighbors of each square in the grid
    size_t rows = nRows();
    size_t cols = nCols();
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            (j == 0) ? data[i][j].setLeft(NULL) : data[i][j].setLeft(&data[i][j - 1]);
            (j == cols - 1) ? data[i][j].setRight(NULL) : data[i][j].setRight(&data[i][j + 1]);
            (i == 0) ? data[i][j].setAbove(NULL) : data[i][j].setAbove(&data[i - 1][j]);
            (i == rows - 1) ? data[i][j].setBelow(NULL) : data[i][j].setBelow(&data[i + 1][j]);
            data[i][j].setPosition(i, j);
        }
    }
}

/*
 * Get *all* the words in the puzzle from scratch, and assign numbers to squares (if they are at the start of a word)
 * along the way. Words are stored in logical (numerical) order; squares comprising each word are stored in order.
 */
void PuzzleGrid::determineWords() {

    gridWords.resize(2);
    gridWords[grid::wordtype::ACROSS].clear();
    gridWords[grid::wordtype::DOWN].clear();

    // A square is the start of a new across/down word iff it doesn't have a (white) neighbor to the left/above.
    int currNum = 1;
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            // Reset all the numbers.
            data[i][j].setNumber(-1);

            if (data[i][j].isSolid()) continue;

            bool isNumberSet = false;
            // Start of a new across word
            if (j == 0 || data[i][j - 1].isSolid()) {
                data[i][j].setNumber(currNum);
                isNumberSet = true;
                GridWord newWord = {true, {&data[i][j]}};
                gridWords[grid::wordtype::ACROSS].push_back(newWord);
                currNum++;
            }
            // Start of a new down word
            if (i == 0 || data[i - 1][j].isSolid()) {
                if (!isNumberSet) {
                    data[i][j].setNumber(currNum);
                    currNum++;
                }
                GridWord newWord = {false, {&data[i][j]}};
                gridWords[grid::wordtype::DOWN].push_back(newWord);
            }
        }
    }

    // Set the <word> member variables in each Square. I wish this code could be less repetitive; the only reason why it
    // isn't is the dependence on getRight() vs. getBelow().
    int wordtype = grid::wordtype::ACROSS;
    for (size_t i = 0; i < gridWords[wordtype].size(); i++) {
        Square* curr = gridWords[wordtype][i].squares[0];
        while (true) {
            curr->setWord(i, wordtype);
            curr->setIndexOfCharInWord(gridWords[wordtype][i].squares.size() - 1, wordtype);
            curr = curr->getRight();
            if (curr == NULL) break;
            if (curr->isSolid()) break;
            gridWords[wordtype][i].squares.push_back(curr);
        }
    }
    wordtype = grid::wordtype::DOWN;
    for (size_t i = 0; i < gridWords[wordtype].size(); i++) {
        Square* curr = gridWords[wordtype][i].squares[0];
        while (true) {
            curr->setWord(i, wordtype);
            curr->setIndexOfCharInWord(gridWords[wordtype][i].squares.size() - 1, wordtype);
            curr = curr->getBelow();
            if (curr == NULL) break;
            if (curr->isSolid()) break;
            gridWords[wordtype][i].squares.push_back(curr);
        }
    }
}

/*
 * Clear all data from the grid.
 */
void PuzzleGrid::clear() {
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j].clearData();
        }
    }
    queue_draw();
}


// ====================================== WORD LOGIC =======================================


GridWord& PuzzleGrid::getWordFromSquare(Square* sq, int wordtype) {
    size_t wordIndex = sq->getWord(wordtype);
    return gridWords[wordtype][wordIndex];
}


// =================================== SYMMETRY CHECKERS ===================================

// TODO: Maybe write a function "getSymmetricEquivalent(int mode)" to get the equivalent square to (i,j).

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

// =================================== EVENT HELPER FUNCS ===================================

bool PuzzleGrid::areSquareIndicesValid(const std::array<int, 2>& indices) const {

    auto [ind_i, ind_j] = indices;
    // Comparing an int and size_t is safe as long as the int is zero or positive.
    if (ind_i < 0 || ind_j < 0 || (size_t)ind_i >= nRows() || (size_t)ind_j >= nCols()) return false;
    return true;
}

/*
 * Could also just return a Square*.
 */
std::array<int, 2> PuzzleGrid::getNextLogicalSquare(const std::array<int, 2>& indices, const int wordtype,
                                                    bool stayWithinWord) const {

    if (!areSquareIndicesValid(indices)) return {-1, -1};

    // Determine which word this square belongs to.
    auto [ind_i, ind_j] = indices;

    // TODO: Even if the input is a black square, get the next white square.
    if (data[ind_i][ind_j].isSolid()) return {-1, -1};

    size_t wordIndex = data[ind_i][ind_j].getWord(wordtype);
    size_t charIndex = data[ind_i][ind_j].getIndexOfCharInWord(wordtype);
    GridWord word = gridWords[wordtype][wordIndex];
    if (charIndex < word.squares.size() - 1) {
        return word.squares[charIndex + 1]->getPosition();
    }
    if (stayWithinWord) return word.squares[word.squares.size() - 1]->getPosition();

    // Get beginning of next word.
    GridWord nextWord = gridWords[wordtype][mod(wordIndex + 1, gridWords[wordtype].size())];
    return nextWord.squares[0]->getPosition();

    return {-1, -1}; // shouldn't get here
}

std::array<int, 2> PuzzleGrid::getPreviousLogicalSquare(const std::array<int, 2>& indices, const int wordtype,
                                                        bool stayWithinWord) const {

    if (!areSquareIndicesValid(indices)) return {-1, -1};

    // Determine which word this square belongs to.
    auto [ind_i, ind_j] = indices;

    if (data[ind_i][ind_j].isSolid()) return {-1, -1};

    int wordIndex = data[ind_i][ind_j].getWord(wordtype);
    int charIndex = data[ind_i][ind_j].getIndexOfCharInWord(wordtype);
    GridWord word = gridWords[wordtype][wordIndex];

    if (charIndex > 0) {
        return word.squares[charIndex - 1]->getPosition();
    }
    if (stayWithinWord) return word.squares[0]->getPosition();

    // Get last of previous word.
    GridWord prevWord = gridWords[wordtype][mod(wordIndex - 1, gridWords[wordtype].size())];
    return prevWord.squares[prevWord.squares.size() - 1]->getPosition();

    return {-1, -1}; // shouldn't get here
}

std::array<int, 2> PuzzleGrid::getNextLogicalEmptySquare(const std::array<int, 2>& indices, const int wordtype,
                                                         bool stayWithinWord) const {

    if (!areSquareIndicesValid(indices)) return {-1, -1};

    // Determine which word this square belongs to.
    auto [ind_i, ind_j] = indices;

    if (data[ind_i][ind_j].isSolid()) return {-1, -1};

    size_t wordIndex = data[ind_i][ind_j].getWord(wordtype);
    size_t charIndex = data[ind_i][ind_j].getIndexOfCharInWord(wordtype);
    Square* sq;

    if (stayWithinWord) {
        int nChars = gridWords[wordtype][wordIndex].squares.size();
        for (int i = 1; i < nChars; i++) {
            // (charIndex + i) gets implicitly converted to an int, but this shouldn't be a problem since it's > 0
            sq = gridWords[wordtype][wordIndex].squares[mod(charIndex + i, nChars)];
            if (sq->isEmpty()) {
                return sq->getPosition();
            }
        }
        return indices;
    }

    for (size_t i = charIndex + 1; i < gridWords[wordtype][wordIndex].squares.size(); i++) {
        sq = gridWords[wordtype][wordIndex].squares[i];
        if (sq->isEmpty()) {
            return sq->getPosition();
        }
    }

    size_t currWord = wordIndex;
    size_t nWords = gridWords[wordtype].size();
    size_t nChars;
    while (true) {
        currWord = mod(currWord + 1, nWords);
        size_t nChars = gridWords[wordtype][currWord].squares.size();
        for (size_t i = 0; i < nChars; i++) {
            sq = gridWords[wordtype][currWord].squares[i];
            if (sq->isEmpty()) {
                return sq->getPosition();
            }
            // if we're back where we started (i.e. the puzzle is completely filled)
            if (currWord == wordIndex && i == charIndex) return indices;
        }
    }

    return {-1, -1}; // shouldn't get here
}

/*
 * Return the indices of the first empty square in the next logical open (unfilled) word.
 */
std::array<int, 2> PuzzleGrid::getNextLogicalOpenWord(const std::array<int, 2>& indices, const int wordtype) {

    if (!areSquareIndicesValid(indices)) return {-1, -1};

    // Get the next open word.
    auto [ind_i, ind_j] = indices;
    size_t wordIndex = data[ind_i][ind_j].getWord(wordtype);
    size_t currIndex = wordIndex;
    GridWord* currWord = &gridWords[wordtype][wordIndex];
    size_t nWords = gridWords[wordtype].size();
    do {
        currIndex = mod(currIndex + 1, nWords);
        currWord = &gridWords[wordtype][currIndex];
    } while (!currWord->isOpen() && currIndex != wordIndex);

    // If the entire puzzle is filled, do nothing
    if (currIndex == wordIndex && !gridWords[wordtype][wordIndex].isOpen()) return indices;

    // If the entire puzzle is filled except for the current word, get the next open square in the current word.
    if (currIndex == wordIndex) {
        return getNextLogicalEmptySquare(indices, wordtype, true);
    }

    // Get the first empty square in the current word.
    for (auto sq : currWord->squares) {
        if (sq->isEmpty()) return sq->getPosition();
    }

    return indices; // shouldn't get here
}

std::array<int, 2> PuzzleGrid::getNextGeometricSquare(const std::array<int, 2>& indices, guint keyval) {

    auto [ind_i, ind_j] = indices;
    int N = nRows();
    int M = nCols();

    // If there are no white squares in the current row/col, just return the original square so we don't
    // accidentally loop forever in one of the while loops below.
    Square sq;
    int i = ind_i;
    int j = ind_j;
    switch (keyval) {
        case (GDK_KEY_Up):
            do {
                i = mod(i - 1, N);
                sq = data[i][j];
            } while (sq.isSolid() && i != ind_i);
            break;
        case (GDK_KEY_Down):
            do {
                i = mod(i + 1, N);
                sq = data[i][j];
            } while (sq.isSolid() && i != ind_i);
            break;
        case (GDK_KEY_Left):
            do {
                j = mod(j - 1, M);
                sq = data[i][j];
            } while (sq.isSolid() && j != ind_j);
            break;
        case (GDK_KEY_Right):
            do {
                j = mod(j + 1, M);
                sq = data[i][j];
            } while (sq.isSolid() && j != ind_j);
            break;
    }
    return sq.getPosition();
}

// =================================== MISCELLANEOUS ===================================


// =================================== SIGNAL HANDLERS ===================================

void PuzzleGrid::on_focus_out() {
    setSelectedSquare({-1, -1});
    queue_draw();
}

void PuzzleGrid::on_left_click(int n_press, double x, double y) {

    grab_focus(); // since apparently set_focus_on_click() doesn't work
    state::lastCommandIsBackspace = false;

    // Determine which square was clicked.
    std::array<int, 2> indices = mapClickToSquareIndex(x, y);
    // If outside the grid, don't do anything.
    if (!areSquareIndicesValid(indices)) return;

    // Don't allow selection of black squares with left click.
    auto [i, j] = indices;
    if (data[i][j].isSolid()) return;

    setSelectedSquare(indices);

    // Automatically display fill options.
    // mainWindow->generate_word_fills();

    // Request redraw.
    queue_draw();
}

void PuzzleGrid::on_right_click(int n_press, double x, double y) {

    grab_focus(); // since apparently set_focus_on_click() doesn't work
    state::lastCommandIsBackspace = false;

    // Determine which square was clicked.
    std::array<int, 2> indices = mapClickToSquareIndex(x, y);
    // If outside the grid, don't do anything.
    if (!areSquareIndicesValid(indices)) return;

    // Toggle selected square white/black.
    auto [ind_i, ind_j] = indices;
    data[ind_i][ind_j].toggleSolid();
    bool solidState = data[ind_i][ind_j].isSolid();
    // If "Make symmetric" is toggled, make sure that the puzzle remains symmetric.
    if (state::makeSymmetric) {
        size_t rows = nRows();
        size_t cols = nCols();
        switch (state::symmetryMode) {
            // Comparing an int and size_t is safe as long as the int is zero or positive.
            case (grid::symmetry::TWO_TURN):
                if (rows - ind_i - 1 != (size_t)ind_i || cols - ind_j - 1 != (size_t)ind_j)
                    data[rows - ind_i - 1][cols - ind_j - 1].setSolid(solidState);
                break;
            case (grid::symmetry::ONE_TURN):
                if (cols - ind_j - 1 != (size_t)ind_j) data[ind_i][cols - ind_j - 1].setSolid(solidState);
                break;
            case (grid::symmetry::MIRROR_UD):
                if (rows - ind_i - 1 != (size_t)ind_i) data[rows - ind_i - 1][ind_j].setSolid(solidState);
                break;
            case (grid::symmetry::MIRROR_LR):
                if (cols - ind_j - 1 != (size_t)ind_j) data[ind_i][cols - ind_j - 1].setSolid(solidState);
                break;
        }
    }

    determineWords(); // there may have been unexpected global changes

    // If we toggled the currently selected square, change the selected square to the next logical square.
    // TODO: Have yet to amend getNextLogicalSquare() appropriately. Currently, the puzzle just deselects (no square
    // is selected.)
    std::array<int, 2> currentSquare = getSelectedSquare();
    if (indices == currentSquare) {
        indices = getNextLogicalSquare(indices, getWordMode());
        setSelectedSquare(indices);
    }

    // Update summary page.
    rightMenuContainer->updateSummaryStats();

    // Request redraw.
    queue_draw();
}

/*
 * Based on which square is selected, set the rendering properties of the square appropriately, along with the word
 * it's contained in.
 */
void PuzzleGrid::renderSelectedSquare() {

    // De-select all squares first.
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j].setSelectionStatus(cell::state::UNSELECTED);
        }
    }

    if (!areSquareIndicesValid(getSelectedSquare())) return;

    // Mark the squares in the current word (either across or down.)
    GridWord* word;
    auto [ind_i, ind_j] = getSelectedSquare();
    int wordtype = getWordMode();
    word = &gridWords[wordtype][data[ind_i][ind_j].getWord(wordtype)];

    for (auto sq : word->squares) {
        sq->setSelectionStatus(cell::state::HIGHLIGHTED);
    }
    // Mark this square as selected.
    data[ind_i][ind_j].setSelectionStatus(cell::state::SELECTED);
}

GridWord* PuzzleGrid::getSelectedWord() {
    if (!areSquareIndicesValid(getSelectedSquare())) return NULL;
    auto [ind_i, ind_j] = getSelectedSquare();
    int wordtype = getWordMode();
    return &gridWords[wordtype][data[ind_i][ind_j].getWord(wordtype)];
}

/*
 * Given the pixel locations of a click, determine which square was clicked.
 *
 * WARNING: The mapping only works if the initial window size is not larger than what can fit on the current device
 * screen. Otherwise, some implicit resizing goes on behind the scenes in a way that I don't quite understand yet,
 * and this mapping only works properly after window gets resized.
 */
std::array<int, 2> PuzzleGrid::mapClickToSquareIndex(double x, double y) {

    // Negative numbers get rounded up, positive numbers get rounded down; just return -1 or a number greater than
    // the max index if click is outside the grid.
    int i = (y - yStart) < 0 ? -1 : std::floor((y - yStart) / squareSize);
    int j = (x - xStart) < 0 ? -1 : std::floor((x - xStart) / squareSize);
    return {i, j};
}

/*
 * "Keyboard events are first sent to the top-level window (Gtk::Window), where it will be checked for any keyboard
 * shortcuts that may be set. Then it is sent to the widget which has focus; the event will propagate to the parent
 * until it reaches the top-level widget, or until you stop the propagation by returning true from an event handler.
 * Returning true indicates that the signal has been fully handled." Returning true also possibly prevents the
 * default handler from running.
 *
 * <keyval> = GDK_KEY_1, GDK_KEY_Escape, etc. See [https://gitlab.gnome.org/GNOME/gtk/-/blob/main/gdk/gdkkeysyms.h].
 * <keycode> = the hardware keycode; an identifying number for a physical key (i.e. "1" and "!" have the same
 * keycode)
 * <state> = Gdk::ModifierType::SHIFT_MASK, etc.
 * <phase> = refers to the phase of event handling in which handler is called; Gtk::PropagationPhase::CAPTURE, etc. See
 * [https://stackoverflow.com/a/4616720].
 *      - Capturing phase – the event goes down to the element.
 *      - Target phase – the event reached the target element.
 *      - Bubbling phase – the event bubbles up from the element.
 */
bool PuzzleGrid::on_key_press(guint keyval, guint keycode, Gdk::ModifierType state, const Glib::ustring& phase) {

    // std::cerr << phase << std::endl;

    if (phase != "bubble") return false;
    // If we press keys in any other part of the window.
    if (!has_focus()) return false;

    // Detect hotkeys
    if (state == Gdk::ModifierType::CONTROL_MASK) {
        // Ctrl-P: print
        // Ctrl-S: save
        // Ctrl-O: load (open)
        return true;
    } else if (state == Gdk::ModifierType::SHIFT_MASK) {
        switch (keyval) {
            case (GDK_KEY_P):
                // Toggle pencil
                // TODO: need to toggle button
                state::pencilSelected = !state::pencilSelected;
                break;
            case (GDK_KEY_F):
                // TODO: Generate fills
                break;
        }
        return true;
    }

    // Could also use gdk_keyval_name(keyval)
    switch (keyval) {
        case (GDK_KEY_Tab):
            // Skip to the next unfilled (text-less) square in the next logical word; do nothing if puzzle is full.
            setSelectedSquare(getNextLogicalOpenWord(getSelectedSquare(), getWordMode()));
            break;
        case (GDK_KEY_space):
            // `Space`: Toggle across/down.
            setWordMode(mod(getWordMode() + 1, 2)); // replace 2 with number of wordtypes if necessary
            break;
        case (GDK_KEY_Delete):
        case (GDK_KEY_BackSpace): {
            // Backspace: clear the current square, and stay in the current square. If the previous command was
            // also a backspace, clear the current square *and* go back a square. The idea is to allow the user to
            // continually clear the current square and go backward upon multiple backspaces.
            auto [i, j] = getSelectedSquare();
            Glib::ustring currData = data[i][j].getData();
            data[i][j].clearData();
            if (state::lastCommandIsBackspace || currData == "")
                setSelectedSquare(getPreviousLogicalSquare(getSelectedSquare(), getWordMode(), true));
            state::lastCommandIsBackspace = true;
        } break;
        case (GDK_KEY_Escape):
            // TODO: `Esc`: Enter more than one letter in a box. Hit Esc again or `Enter' to exit this mode.
            // This should make the square a different color, and instead of auto-skipping to the next square upon
            // hitting a key, stays in the current square and appends to the current square data. Would be better if the
            // square turned into a Gtk::Entry, so user has cursor freedom.
            break;
        case (GDK_KEY_Return):
            // TODO: Search fills according to user-defined criteria. Generate and display a list of top 10 fills.
            break;
        case (GDK_KEY_Up):
            // Arrow keys: move one box in the corrresponding direction (skipping black squares.) This moves in
            // the direction you'd expect in the grid, not necessarily to the next logical square.
            setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_Up));
            break;
        case (GDK_KEY_Down):
            setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_Down));
            break;
        case (GDK_KEY_Left):
            setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_Left));
            break;
        case (GDK_KEY_Right):
            setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_Right));
            break;
        default: {
            // Otherwise, assume that the user has entered a symbol that is allowed to be in a cell.
            // https://developer-old.gnome.org/glibmm/unstable/classGlib_1_1ustring.html
            gunichar character = gdk_keyval_to_unicode(keyval);
            auto [i, j] = getSelectedSquare();
            if (!areSquareIndicesValid({i, j})) return true;
            Glib::ustring text(1, character); // args = number of characters, ucUCS-4 code point
            if (text == "") return true;      // if the key pressed doesn't result in a char, like shift, ctrl, etc.
            text = text.uppercase();
            data[i][j].setData(text);
            data[i][j].setUtensil(state::pencilSelected ? cell::PENCIL : cell::PEN);

            // Go to the next unfilled square in the current word, staying at the current square if the word is
            // completely filled. TODO: detect if we're "revisiting" a filled word, may still want to auto-skip to
            // next square in that case.
            setSelectedSquare(getNextLogicalEmptySquare(getSelectedSquare(), getWordMode(), true)); // stay within word
        } break;
    }

    queue_draw();

    return true;
}