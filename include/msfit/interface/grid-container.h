#pragma once

#include "gtkmm/paned.h"

#include "msfit/interface/bottom-menu-container.h"

namespace MsFitInterface {

class GridContainer : public Gtk::Paned {
  public:
    GridContainer();
    ~GridContainer() {}

    int margin = 10;
    BottomMenuContainer bottomMenuContainer;
};
} // namespace MsFitInterface