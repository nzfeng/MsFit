#include "msfit/utilities/drawing_utils.h"

/*
 * Draw the given text string in the center of a polygon whose corners' 2D positions are given.
 * https://developer-old.gnome.org/gtkmm-tutorial/stable/sec-drawing-text.html.en
 */
void draw_text(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, const Glib::ustring& text,
               const std::vector<Vector2>& cellCorners) {

    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
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

    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    auto layout = drawingArea.create_pango_layout(text);

    layout->set_font_description(font);

    int text_width;
    int text_height;

    // get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->move_to((center[0] - text_width) / 2, (center[1] - text_height) / 2);

    layout->show_in_cairo_context(cr);
}