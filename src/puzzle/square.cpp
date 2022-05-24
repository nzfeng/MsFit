#include "msfit/puzzle/square.h"
#include "msfit/utilities/drawing_utils.h"
#include "msfit/utilities/vector2.h"

// Square::Square() { Square(WHITE, ""); }

Square::Square(int isSolid_, Glib::ustring data_, int width_, int selectionStatus_)
    : solid(isSolid_), data(data_), width(width_), selectionStatus(selectionStatus_) {}

void Square::draw(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, size_t size, size_t x,
                  size_t y) {

    // Scale the user-space coordinate system s.t. the width and height of the widget are both equal to 1.0 units.
    // cr->scale(size, size);

    if (solid) {
        // Simply draw a black square.
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->rectangle(x, y, size, size);
        cr->fill();
    } else {
        // Draw a colored square with a gray outline by first drawing a gray square, then drawing a slightly smaller
        // colored square on top.
        cr->set_source_rgb(0.8, 0.8, 0.8);
        cr->rectangle(x, y, size, size);
        cr->fill();

        switch (selectionStatus) {
        case (cell::UNSELECTED):
            cr->set_source_rgb(1.0, 1.0, 1.0); // white
            break;
        case (cell::SELECTED):
            cr->set_source_rgb(1.0, 0.0, 0.0); // pale yellow
            break;
        case (cell::HIGHLIGHTED):
            cr->set_source_rgb(0.0, 0.0, 0.5); // pale blue
            break;
        }
        // border is 0.5px wide inside each square, 1px wide when squares are next to each other.
        cr->rectangle(x + 0.5, y + 0.5, size - 1, size - 1);
        cr->fill();

        // Render text if needed.
        // TODO: Perhaps template Vector2 code.
        std::vector<Vector2> corners = {{static_cast<double>(x), static_cast<double>(y)},
                                        {static_cast<double>(x + size), static_cast<double>(y)},
                                        {static_cast<double>(x + size), static_cast<double>(y + size)},
                                        {static_cast<double>(x), static_cast<double>(y + size)}};
        draw_text(drawingArea, cr, data, corners);
        draw_number(drawingArea, cr, number, corners);
    }
}