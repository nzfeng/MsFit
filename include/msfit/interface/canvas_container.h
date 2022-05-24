#pragma once

#include <gtkmm/aspectframe.h>

#include "msfit/puzzle/puzzle_grid.h"

class CanvasContainer : public Gtk::AspectFrame {
  public:
    CanvasContainer();
    virtual ~CanvasContainer() {}
};