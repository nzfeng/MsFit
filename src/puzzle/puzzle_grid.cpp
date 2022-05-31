#include <gtkmm/accelerator.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gestureclick.h>

#include "msfit/puzzle/puzzle_grid.h"
#include "msfit/utilities/state.h"
#include "msfit/utilities/utils.h"

// Constructors
PuzzleGrid::PuzzleGrid() { PuzzleGrid(grid::params::N_ROWS, grid::params::N_COLS); }

PuzzleGrid::PuzzleGrid(size_t nRows_, size_t nCols_) {
    setSize(nRows_, nCols_);
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

    auto keyHandler = Gtk::EventControllerKey::create();
    keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "target"),
                                             false);
    renderedGrid.add_controller(keyHandler);
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
    getWords();
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
            data[i][j].draw(renderedGrid, cr, squareSize, x + squareSize * j, y + squareSize * i);
        }
    }
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
void PuzzleGrid::getWords() {

    acrossWords.clear();
    downWords.clear();
    // A square is the start of a new across/down word iff it doesn't have a (white) neighbor to the left/above.
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
                GridWord newWord = {true, {&data[i][j]}};
                acrossWords.push_back(newWord);
                currNum++;
            }
            // Start of a new down word
            if (i == 0 || data[i - 1][j].isSolid()) {
                if (!isNumberSet) {
                    data[i][j].setNumber(currNum);
                    currNum++;
                }
                GridWord newWord = {false, {&data[i][j]}};
                downWords.push_back(newWord);
            }
        }
    }

    // Set the <word> member variables in each Square.
    for (size_t i = 0; i < acrossWords.size(); i++) {
        Square* curr = acrossWords[i].squares[0];
        while (true) {
            curr->setAcrossWord(i);
            curr->setAcrossWordIndex(acrossWords[i].squares.size() - 1);
            curr = curr->getRight();
            if (curr == NULL) break;
            if (curr->isSolid()) break;
            acrossWords[i].squares.push_back(curr);
        }
    }
    for (size_t i = 0; i < downWords.size(); i++) {
        Square* curr = downWords[i].squares[0];
        while (true) {
            curr->setDownWord(i);
            curr->setDownWordIndex(downWords[i].squares.size() - 1);
            curr = curr->getBelow();
            if (curr == NULL) break;
            if (curr->isSolid()) break;
            downWords[i].squares.push_back(curr);
        }
    }
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
std::array<int, 2> PuzzleGrid::getNextLogicalSquare(const std::array<int, 2>& indices, bool isAcross) const {

    if (!areSquareIndicesValid(indices)) return {-1, -1};

    // Determine which word this square belongs to.
    auto [ind_i, ind_j] = indices;

    if (isAcross) {
        size_t wordIndex = data[ind_i][ind_j].getAcrossWord();
        size_t charIndex = data[ind_i][ind_j].getAcrossWordIndex();
        assert(wordIndex >= 0 && wordIndex < acrossWords.size());
        assert(charIndex >= 0 && charIndex < acrossWords[wordIndex].squares.size());
        GridWord word = acrossWords[wordIndex];
        if (charIndex < word.squares.size() - 1) {
            return {ind_i, ind_j + 1};
        }
        // Get beginning of next word.
        GridWord nextWord = acrossWords[mod(wordIndex + 1, acrossWords.size())];
        return nextWord.squares[0]->getPosition();
    } else {
        size_t wordIndex = data[ind_i][ind_j].getDownWord();
        size_t charIndex = data[ind_i][ind_j].getDownWordIndex();
        assert(wordIndex >= 0 && wordIndex < downWords.size());
        assert(charIndex >= 0 && charIndex < downWords[wordIndex].squares.size());
        GridWord word = downWords[wordIndex];
        if (charIndex < word.squares.size() - 1) {
            return {ind_i + 1, ind_j};
        }
        // Get beginning of next word.
        GridWord nextWord = downWords[mod(wordIndex + 1, downWords.size())];
        return nextWord.squares[0]->getPosition();
    }
    return {-1, -1}; // shouldn't get here
}

std::array<int, 2> PuzzleGrid::getPreviousLogicalSquare(const std::array<int, 2>& indices, bool isAcross) const {

    if (!areSquareIndicesValid(indices)) return {-1, -1};

    // Determine which word this square belongs to.
    auto [ind_i, ind_j] = indices;

    if (isAcross) {
        int wordIndex = data[ind_i][ind_j].getAcrossWord();
        int charIndex = data[ind_i][ind_j].getAcrossWordIndex();
        GridWord word = acrossWords[wordIndex];
        if (charIndex > 0) {
            return {ind_i, ind_j - 1};
        }
        // Get last of previous word.
        GridWord prevWord = acrossWords[mod(wordIndex - 1, acrossWords.size())];
        return prevWord.squares[prevWord.squares.size() - 1]->getPosition();
    } else {
        int wordIndex = data[ind_i][ind_j].getDownWord();
        int charIndex = data[ind_i][ind_j].getDownWordIndex();
        GridWord word = downWords[wordIndex];
        if (charIndex > 0) {
            return {ind_i - 1, ind_j};
        }
        // Get last of previous word.
        GridWord prevWord = downWords[mod(wordIndex - 1, downWords.size())];
        return prevWord.squares[prevWord.squares.size() - 1]->getPosition();
    }
    return {-1, -1}; // shouldn't get here
}

std::array<int, 2> PuzzleGrid::getNextGeometricSquare(const std::array<int, 2>& indices, guint keyval) {

    auto [i, j] = indices;
    int N = nRows();
    int M = nCols();
    Square sq;
    switch (keyval) {
        // TODO: need to implement wrap-around
    case (GDK_KEY_uparrow):
        do {
            sq = data[mod(i - 1, N)][j];
        } while (!sq.isSolid());
        break;
    case (GDK_KEY_downarrow):
        do {
            sq = data[mod(i + 1, N)][j];
        } while (!sq.isSolid());
        break;
    case (GDK_KEY_leftarrow):
        do {
            sq = data[i][mod(j - 1, M)];
        } while (!sq.isSolid());
        break;
    case (GDK_KEY_rightarrow):
        do {
            sq = data[i][mod(j + 1, M)];
        } while (!sq.isSolid());
        break;
    }
    return sq.getPosition();
}

// =================================== SIGNAL HANDLERS ===================================

void PuzzleGrid::on_left_click(int n_press, double x, double y) {

    // Determine which square was clicked.
    std::array<int, 2> indices = mapClickToSquareIndex(x, y);
    // If outside the grid, don't do anything.
    if (!areSquareIndicesValid(indices)) return;

    setSelectedSquare(indices);

    // Request redraw.
    renderedGrid.queue_draw();
}

void PuzzleGrid::on_right_click(int n_press, double x, double y) {

    // Determine which square was clicked.
    std::array<int, 2> indices = mapClickToSquareIndex(x, y);
    // If outside the grid, don't do anything.
    if (!areSquareIndicesValid(indices)) return;

    // Toggle selected square white/black; re-number words; if "Make symmetric" is toggled, make sure that the
    // puzzle remains symmetric.
    auto [ind_i, ind_j] = indices;
    data[ind_i][ind_j].toggleSolid();
    if (state::makeSymmetric) {
        size_t rows = nRows();
        size_t cols = nCols();
        switch (state::symmetryMode) {
        case (grid::symmetry::TWO_TURN):
            data[rows - ind_i - 1][cols - ind_j - 1].toggleSolid();
            break;
        case (grid::symmetry::ONE_TURN):
            data[ind_i][cols - ind_j - 1].toggleSolid();
            break;
        case (grid::symmetry::MIRROR_UD):
            data[rows - ind_i - 1][ind_j].toggleSolid();
            break;
        case (grid::symmetry::MIRROR_LR):
            data[ind_i][cols - ind_j - 1].toggleSolid();
            break;
        }
    }

    // If we toggled the currently selected square, change the selected square to the next logical square.
    std::array<int, 2> currentSquare = getSelectedSquare();
    if (indices == currentSquare) {
        indices = getNextLogicalSquare(indices, isAcrossSelected());
    }

    getWords(); // there may have been unexpected global changes
    setSelectedSquare(indices);

    // Request redraw.
    renderedGrid.queue_draw();
}

/*
 * Based on which square is selected, set the rendering properties of the square appropriately, along with the word it's
 * contained in.
 */
void PuzzleGrid::renderSelectedSquare() {

    if (!areSquareIndicesValid(selectedSquare)) return;

    // De-select all squares first.
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j].setSelectionStatus(cell::state::UNSELECTED);
        }
    }
    // Mark the squares in the current word (either across or down.)
    GridWord* word;
    auto [ind_i, ind_j] = getSelectedSquare();
    if (isAcrossSelected()) {
        word = &acrossWords[data[ind_i][ind_j].getAcrossWord()];

    } else {
        word = &downWords[data[ind_i][ind_j].getDownWord()];
    }
    for (auto sq : word->squares) {
        sq->setSelectionStatus(cell::state::HIGHLIGHTED);
    }
    // Mark this square as selected.
    data[ind_i][ind_j].setSelectionStatus(cell::state::SELECTED);
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
 * Keyboard events are first sent to the top-level window (Gtk::Window), where it will be checked for any keyboard
 * shortcuts that may be set. Then it is sent to the widget which has focus; the event will propagate to the parent
 * until it reaches the top-level widget, or until you stop the propagation by returning true from an event handler.
 * Returning true indicates that the signal has been fully handled.
 *
 * <keyval> = GDK_KEY_1, GDK_KEY_Escape, etc. See [https://gitlab.gnome.org/GNOME/gtk/-/blob/main/gdk/gdkkeysyms.h].
 * <state> = Gdk::ModifierType::SHIFT_MASK, etc.
 * <phase> = refers to the phase of event handling in which handler is called; Gtk::PropagationPhase::CAPTURE, etc.
 * See [https://stackoverflow.com/a/4616720].
 *      - Capturing phase – the event goes down to the element.
 *      - Target phase – the event reached the target element.
 *      - Bubbling phase – the event bubbles up from the element.
 */
bool PuzzleGrid::on_key_press(guint keyval, guint keycode, Gdk::ModifierType state, const Glib::ustring& phase) {

    // Ignore key releases.
    if (keycode != GDK_KEY_PRESS) return true;

    switch (keyval) {
    case (GDK_KEY_Tab):
        // TODO: Skip to the next unfilled square; do nothing if puzzle is full.
        break;
    case (GDK_KEY_space):
        // TODO: `Space`: Toggle across/down.
        break;
    case (GDK_KEY_BackSpace):
        // TODO: Backspace: clear the current square, or go back a square if the current one is empty. TODO: Upon
        // multiple backspaces, continually clear the current square and go backward.
        break;
    case (GDK_KEY_Delete):
        // TODO: same as backspace
        break;
    case (GDK_KEY_Escape):
        // TODO: `Esc`: Enter more than one letter in a box. Hit Esc again or `Enter' to exit this mode.
        break;
    case (GDK_KEY_Return):
        // TODO: Search fills according to user-defined criteria. Generate and display a list of top 10 fills.
        break;
    case (GDK_KEY_uparrow):
        // Arrow keys: move one box in the corrresponding direction (skipping black squares.) This moves in
        // the direction you'd expect in the grid, not necessarily to the next word, in order.
        setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_uparrow));
        break;
    case (GDK_KEY_downarrow):
        setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_downarrow));
        break;
    case (GDK_KEY_leftarrow):
        setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_leftarrow));
        break;
    case (GDK_KEY_rightarrow):
        setSelectedSquare(getNextGeometricSquare(getSelectedSquare(), GDK_KEY_rightarrow));
        break;
    }

    // Else, assume the user meant to enter a character in the current square.
    // https://developer-old.gnome.org/gtkmm/stable/namespaceGtk_1_1Accelerator.html#details
    Glib::ustring character = Gtk::Accelerator::get_label(keyval, Gdk::ModifierType::SHIFT_MASK);
    // TODO: Any letter key: Enter the capital letter in the currently selected box (if any); render in gray if the
    // pencil icon is selected.

    renderedGrid.queue_draw();

    return true;
}