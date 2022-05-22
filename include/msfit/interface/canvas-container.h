#pragma once

#include "gtkmm/aspectframe.h"

namespace MsFitInterface {

class CanvasContainer : public Gtk::AspectFrame {
  public:
    CanvasContainer();
    ~CanvasContainer() {}

    int margin = 10;
};
} // namespace MsFitInterface