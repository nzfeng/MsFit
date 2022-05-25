#include "msfit/utilities/drawing_utils.h"

/*
 * Draw the given text string in the center of a polygon whose corners' 2D positions (vertices) are given.
 * https://developer-old.gnome.org/gtkmm-tutorial/stable/sec-drawing-text.html.en
 */
void draw_text(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, const Glib::ustring& text,
               const std::vector<Vector2>& cellCorners) {

    cr->set_source_rgb(0.0, 0.0, 0.0);
    Pango::FontDescription font;

    // Get cell geometry parameters.
    Vector2 center = {0, 0};
    for (auto corner : cellCorners) {
        center += corner;
    }
    center /= cellCorners.size();
    double radius = 0;
    for (auto corner : cellCorners) {
        radius = std::max(radius, (corner - center).norm());
    }

    // Set the font size so that its width/height don't exceed a certain proportion of the cell.
    double font_size = std::min(radius, radius / text.size()); // I think font size corresponds to the font height?
    font.set_family("sans");
    font.set_weight(Pango::Weight::BOLD);
    font.set_size(font_size);

    int text_width, text_height;
    auto layout = drawingArea.create_pango_layout(text);
    layout->set_font_description(font);

    // get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->move_to((center[0] - text_width) / 2, (center[1] - text_height) / 2);

    layout->show_in_cairo_context(cr);
}

/*
 * Draw the given number in the upper left corner of a cell.
 * TODO: Think about placement for other polygons.
 */
void draw_number(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, const int& number,
                 const std::vector<Vector2>& cellCorners) {

    if (number == -1) return;

    cr->set_source_rgb(0.0, 0.0, 0.0); // make sure text is black
    Pango::FontDescription font;
    Glib::ustring text = std::to_string(number);

    // Get cell geometry parameters.
    // TODO: These computations are somewhat redundant; combine with draw_text()?
    Vector2 center = {0, 0};
    Vector2 upperLeft; // upper-leftmost position
    double leftMost = std::numeric_limits<double>::infinity();
    double upperMost = std::numeric_limits<double>::infinity();
    for (auto corner : cellCorners) {
        center += corner;
        if (corner[0] < leftMost && corner[1] < upperMost) {
            leftMost = corner[0];
            upperMost = corner[1];
            upperLeft = corner;
        }
    }
    center /= cellCorners.size();

    double radius = 0;
    for (auto corner : cellCorners) {
        radius = std::max(radius, (corner - center).norm());
    }

    double scale = 0.3;
    double font_size = radius * scale;
    font.set_family("sans");
    font.set_weight(Pango::Weight::NORMAL);
    font.set_size(font_size * PANGO_SCALE);

    int text_width, text_height;
    auto layout = drawingArea.create_pango_layout(text);
    layout->set_font_description(font);
    layout->get_pixel_size(text_width, text_height);

    // Position the text roughly in the upper left
    // double t = 0.05;
    // Vector2 pos = (1. - t) * upperLeft + t * center;
    Vector2 pos = upperLeft;
    cr->move_to(pos[0] + text_width / 4, pos[1] + text_height / 4);

    // This places the text so that its upper left corner is at the position set above.
    layout->show_in_cairo_context(cr);
}