// Define a custom Button class, so I can customize the appearance more easily.

#pragma once

#include <gtkmm/button.h>
#include <gtkmm/label.h>

class MsFitButton : public Gtk::Button {

  public:
    MsFitButton();
    virtual ~MsFitButton();

    void setText(const Glib::ustring& text);
    void setFontSize(int font_size = 10);
    int getFontSize();
    void setFontFamily(Glib::ustring family = "sans");
    // void setFontFace();

    // PangoWeight = an enumeration specifying the weight (boldness) of a font.
    // See [https://docs.gtk.org/Pango/enum.Weight.html] for options.
    void setFontWeight(Pango::Weight weight = Pango::Weight::NORMAL);

    // void setFontColor();
    // void setFillColor();
    // void setTextAlignment();
};