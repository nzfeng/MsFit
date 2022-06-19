#include "msfit/interface/msfit_button.h"

MsFitButton::MsFitButton() {}

MsFitButton::~MsFitButton() {}

void MsFitButton::setFontFamily(Glib::ustring family) {
    auto layout = get_pango_context();
    auto font = layout->get_font_description();
    font.set_family(family);
    layout->set_font_description(font);
}

void MsFitButton::setFontSize(int font_size) {
    // auto layout = get_pango_context();
    // // Despite what the online documentation says, get_font_description() doesn't seem to return a pointer.
    // auto font = layout->get_font_description();
    // font.set_size(font_size * PANGO_SCALE);
    // layout->set_font_description(font);

    Gtk::Label test("test");
    auto layout = test.get_pango_context();
    auto font = layout->get_font_description();
    font.set_size(font_size * PANGO_SCALE);
    layout->set_font_description(font);
    set_label(test);
}

int MsFitButton::getFontSize() {
    auto layout = get_pango_context();
    auto font = layout->get_font_description();
    return font.get_size() / PANGO_SCALE;
}

void MsFitButton::setFontWeight(Pango::Weight weight) {
    auto layout = get_pango_context();
    auto font = layout->get_font_description();
    font.set_weight(weight);
    layout->set_font_description(font);
}