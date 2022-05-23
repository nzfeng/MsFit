/*
 * A Square cell. Used in the PuzzleGrid data structure.
 *
 * In terms of visualization, each square will either be:
 *  - BLACK = entirely black
 *  - WHITE = white with a black border, possibly with text in the center, and a number in the upper left corner.
 */

#include <gtkmm/drawingarea.h>

namespace puzzle {

class Square {

  public:
    Square();
    Square(int state = puzzle::WHITE, Glib::ustring data = "", int width = 20);
    ~Square() {}

    Glib::ustring getData() const;
    void setData(const Glib::ustring& str);
    void setSize(int width);
    int getSize() const;

    void draw(size_t width);

  private:
    // State
    int state = -1;
    Glib::ustring data;
    size_t width;
    Gtk::DrawingArea renderedSquare; // rendered square

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, size_t width);
};
} // namespace puzzle