#pragma once

#include "gtkmm/paned.h"

#include "msfit/interface/right-menu-container.h"

namespace MsFitInterface {

class MainContainer : public Gtk::Paned {

  public:
    MainContainer();
    ~MainContainer() {}

    int margin = 10;
};
} // namespace MsFitInterface