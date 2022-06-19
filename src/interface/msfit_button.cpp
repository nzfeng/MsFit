#include "msfit/interface/msfit_button.h"

MsFitButton::MsFitButton() {}

MsFitButton::~MsFitButton() {}

void MsFitButton::setText(const Glib::ustring& text) {
    Gtk::Label label(text);
    set_child(label);
}

void MsFitButton::setFontFamily(Glib::ustring family) {
    auto label = get_child();
    auto layout = label->get_pango_context();
    auto font = layout->get_font_description();
    font.set_family(family);
    layout->set_font_description(font);
}

void MsFitButton::setFontSize(int font_size) {
    auto label = get_child();
    auto layout = label->get_pango_context();
    auto font = layout->get_font_description();
    font.set_size(font_size * PANGO_SCALE);
    layout->set_font_description(font);
}

int MsFitButton::getFontSize() {
    auto label = get_child();
    auto layout = label->get_pango_context();
    auto font = layout->get_font_description();
    return font.get_size() / PANGO_SCALE;
}

void MsFitButton::setFontWeight(Pango::Weight weight) {
    auto label = get_child();
    auto layout = label->get_pango_context();
    auto font = layout->get_font_description();
    font.set_weight(weight);
    layout->set_font_description(font);
}