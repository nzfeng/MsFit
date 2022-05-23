#pragma once

#include <gtkmm/paned.h>

namespace MsFitInterface {

class GridContainer : public Gtk::Paned {
  public:
    GridContainer();
    ~GridContainer() {}
};
} // namespace MsFitInterface