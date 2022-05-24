#include "msfit/puzzle/square.h"
#include "msfit/utilities/drawing_utils.h"
#include "msfit/utilities/state.h"

// Square::Square() { Square(WHITE, ""); }

Square::Square(int isSolid_, Glib::ustring data_, int width_) : solid(isSolid_), data(data_), width(width_) {}

inline Glib::ustring Square::getData() const { return data; }

inline void Square::setData(const Glib::ustring& str) { data = str; }

inline void Square::setSize(int width_) { width = width_; }

inline int Square::getSize() const { return width; }

inline bool Square::isSolid() const { return solid; }

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
        // Draw a white square with a black outline by first drawing a black square, then drawing a slightly smaller
        // white square on top.
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->rectangle(x, y, size, size);
        cr->fill();
        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->rectangle(x, y, size * 0.9, size * 0.9);
        cr->fill();

        // Render text if needed.
        // TODO: Perhaps template Vector2 code.
        std::vector<Vector2> corners = {{static_cast<double>(x), static_cast<double>(y)},
                                        {static_cast<double>(x + size), static_cast<double>(y)},
                                        {static_cast<double>(x + size), static_cast<double>(y + size)},
                                        {static_cast<double>(x), static_cast<double>(y + size)}};
        draw_text(drawingArea, cr, data, corners);
    }
}