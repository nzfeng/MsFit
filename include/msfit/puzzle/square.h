/*
 * A Square cell. Used in the PuzzleGrid data structure.
 *
 * In terms of visualization, each square will either be:
 *  - BLACK = entirely black
 *  - WHITE = white with a black border, possibly with text in the center, and a number in the upper left corner.
 */

#include <gtkmm/drawingarea.h>
#include <iostream>

#include "msfit/puzzle/grid_word.h"
#include "msfit/utilities/state.h"

// TODO: inherit from Gtk::Entry?
class Square {

  public:
    Square(int isSolid = false, Glib::ustring data = "", int width = 20, int selectionStatus = cell::UNSELECTED);
    ~Square() {}

    Glib::ustring getData() const;
    void setData(const Glib::ustring& str);

    void setSize(int width);
    int getSize() const;

    bool isSolid() const;
    void toggleSolid();

    void setNumber(int num);
    int getNumber() const;

    void setLeft(Square* neighbor);
    void setRight(Square* neighbor);
    void setAbove(Square* neighbor);
    void setBelow(Square* neighbor);
    Square* getLeft() const;
    Square* getRight() const;
    Square* getAbove() const;
    Square* getBelow() const;

    void setPosition(int i, int j);
    std::array<int, 2> getPosition() const;

    // void setDownWord(GridWord* word);
    // GridWord* getDownWord() const;
    // void setAcrossWord(GridWord* word);
    // GridWord* getAcrossWord() const;

    void setWord(size_t index, int wordtype);
    size_t getWord(int wordtype) const;

    void setIndexOfCharInWord(size_t index, int wordtype);
    size_t getIndexOfCharInWord(int wordtype) const;

    void setSelectionStatus(int status);

    bool isEmpty() const;

    // Render this square with the given side length (<width>) on the canvas <cr>, with its upper left corner at the
    // specified position on <cr>, which represents the entire puzzle.
    void draw(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, size_t size, size_t x, size_t y);

  private:
    // State
    bool solid;
    Glib::ustring data;
    size_t width;
    int selectionStatus;
    int number = -1; // only positive if this square is the start of a new word

    // Info about this square within the grid
    Square* left;
    Square* right;
    Square* above;
    Square* below;
    std::array<int, 2> position;

    // Words that this square belongs to
    size_t wordIndex[2];
    // The index of this square within each word.
    size_t charIndex[2];
};

#include "msfit/puzzle/square.ipp"