#include <msfit/engine/square.h>
#include <msfit/utilities/drawing_utils.h>
#include <msfit/utilities/vector2.h>

namespace puzzle {

Square::Square() { Square(puzzle::WHITE, ""); }

Square::Square(int state_, Glib::ustring data_, int width_) : state(state_), data(data_), width(width_) {

    renderedSquare.set_draw_func(sigc::mem_fun(*this, &Square::on_draw));
}

inline Glib::ustring Square::getData() const { return data; }

inline void Square::setData(const Glib::ustring& str) { data = str; }

inline void Square::setSize(int width_) { width = width_; }

inline int Square::getSize() { return width; }

void Square::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, size_t size) {

    // Scale the user-space coordinate system s.t. the width and height of the widget are both equal to 1.0 units.
    // cr->scale(size, size);

    switch (state) {
        case puzzle::BLACK:
            // Paint whole canvas black.
            cr->set_source_rgb(0.0, 0.0, 0.0);
            cr->paint();
            break;
        case puzzle::WHITE:
            // Paint whole canvas white.
            cr->set_source_rgb(1.0, 1.0, 1.0);
            cr->paint();
            // Set line color to black.
            cr->set_source_rgb(0.0, 0.0, 0.0);
            cr->set_line_width(size / 20);
            // Draw black line around the border of the square.
            cr->move_to(0, 0);
            cr->line_to(size, 0);
            cr->line_to(size, size);
            cr->line_to(0, size);
            cr->line_to(0, 0);
            cr->stroke();

            // Render text if needed.
            std::vector<Vector2> corners = {{0, 0}, {size, 0}, {size, size}, {0, size}};
            draw_text(cr, data, corners);
            break;
        default:
            break;
    }
}

} // namespace puzzle