/*
 * A Square cell. Used in the PuzzleGrid data structure.
 *
 * In terms of visualization, each square will either be:
 *  - BLACK = entirely black
 *  - WHITE = white with a black border, possibly with text in the center, and a number in the upper left corner.
 */

#include <gtkmm/drawingarea.h>
#include <iostream>

#include "msfit/utilities/state.h"

class Square {

  public:
    Square(int isSolid = false, Glib::ustring data = "", int width = 20, int selectionStatus = cell::UNSELECTED);
    ~Square() {}

    Glib::ustring getData() const;
    void setData(const Glib::ustring& str);

    void setSize(int width);
    int getSize() const;

    bool isSolid() const;

    void setNumber(int num);
    int getNumber() const;

    void setDownWord(size_t idx);
    size_t getDownWord() const;
    void setAcrossWord(size_t idx);
    size_t getAcrossWord() const;

    // Render this square with the given side length (<width>) on the canvas <cr>, with its upper left corner at the
    // specified position on <cr>, which represents the entire puzzle.
    void draw(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, size_t size, size_t x, size_t y);

  private:
    // State
    bool solid;
    Glib::ustring data;
    size_t width;
    int selectionStatus;
    int number = -1;

    // Info about this square within the grid
    // Square& left;
    // Square& right;
    // Square& above;
    // Square& below;
    // words that this square belongs to -- the indices in the corresponding array of words in the PuzzleGrid object,
    // *not* the actual puzzle numbering
    size_t downWord, acrossWord;
};

#include "msfit/puzzle/square.ipp"