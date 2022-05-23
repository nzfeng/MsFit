#pragma once

#include <gtkmm/paned.h>

namespace interface {

class GridContainer : public Gtk::Paned {
  public:
    GridContainer();
    ~GridContainer() {}
};
} // namespace interface