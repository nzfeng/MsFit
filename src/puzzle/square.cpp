#include "msfit/puzzle/square.h"
#include "msfit/utilities/drawing_utils.h"
#include "msfit/utilities/vector2.h"

// Square::Square() { Square(WHITE, ""); }

Square::Square(int isSolid_, Glib::ustring data_, int width_, int selectionStatus_, int utensil_)
    : solid(isSolid_), data(data_), width(width_), selectionStatus(selectionStatus_), utensil(utensil_) {}

/*
 * Draws directly on the PuzzleGrid drawingArea member.
 */
void Square::draw(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, size_t size, size_t x,
                  size_t y) {

    // Scale the user-space coordinate system s.t. the width and height of the widget are both equal to 1.0 units.
    // cr->scale(size, size);
    std::array<float, 3> color;
    std::array<float, 3> color_outline = theme::color_outline;
    std::array<float, 3> solid_outline = theme::color_solid_outline;
    float border;

    if (solid) {
        // Simply draw a black square with (a darker) gray outline (possibly with yellow outline, if selected)
        std::array<float, 3> color_solid = theme::color_solid;
        switch (selectionStatus) {
            case (cell::UNSELECTED):
                cr->set_source_rgb(solid_outline[0], solid_outline[1], solid_outline[2]);
                cr->rectangle(x, y, size, size);
                cr->fill();

                border = theme::white_border_width;
                cr->set_source_rgb(color_solid[0], color_solid[1], color_solid[2]);
                cr->rectangle(x + 0.5 * border, y + 0.5 * border, size - border, size - border);
                cr->fill();
                break;
            case (cell::SELECTED):
                color = theme::color_selected;
                cr->set_source_rgb(color[0], color[1], color[2]); // yellow outline
                cr->rectangle(x, y, size, size);
                cr->fill();
                cr->set_source_rgb(color_solid[0], color_solid[1], color_solid[2]);
                border = theme::black_border_width;
                cr->rectangle(x + 0.5 * border, y + 0.5 * border, size - border, size - border);
                cr->fill();
                break;
        }
    } else {
        // Draw a colored square with a gray outline by first drawing a gray square, then drawing a slightly smaller
        // colored square on top.
        cr->set_source_rgb(color_outline[0], color_outline[1], color_outline[2]);
        cr->rectangle(x, y, size, size);
        cr->fill();

        switch (selectionStatus) {
            case (cell::UNSELECTED):
                color = theme::color_unselected;
                cr->set_source_rgb(color[0], color[1], color[2]); // white
                break;
            case (cell::SELECTED):
                color = theme::color_selected;
                cr->set_source_rgb(color[0], color[1], color[2]); //  yellow
                break;
            case (cell::HIGHLIGHTED):
                color = theme::color_highlighted;
                cr->set_source_rgb(color[0], color[1], color[2]); // pale blue
                break;
        }
        // border is 0.5px wide inside each square, 1px wide when squares are next to each other.
        border = theme::white_border_width;
        cr->rectangle(x + 0.5 * border, y + 0.5 * border, size - border, size - border);
        cr->fill();

        // Render text if needed.
        // TODO: Perhaps template Vector2 code.
        std::vector<Vector2> corners = {{static_cast<double>(x), static_cast<double>(y)},
                                        {static_cast<double>(x + size), static_cast<double>(y)},
                                        {static_cast<double>(x + size), static_cast<double>(y + size)},
                                        {static_cast<double>(x), static_cast<double>(y + size)}};
        draw_text(drawingArea, cr, data, corners, getUtensil());
        draw_number(drawingArea, cr, number, corners);
    }
}