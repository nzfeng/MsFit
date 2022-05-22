#pragma once

#include "gtkmm/notebook.h"

namespace MsFitInterface {

class RightMenuContainer : public Gtk::Notebook {

  public:
    RightMenuContainer();
    ~RightMenuContainer() {}

    int margin = 10;
    Gtk::Label label_1, label_2;
};
} // namespace MsFitInterface