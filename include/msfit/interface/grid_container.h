#pragma once

#include <gtkmm/paned.h>

class GridContainer : public Gtk::Paned {
  public:
    GridContainer();
    virtual ~GridContainer() {}
};