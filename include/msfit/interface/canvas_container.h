#pragma once

#include <gtkmm/aspectframe.h>

namespace interface {

class CanvasContainer : public Gtk::AspectFrame {
  public:
    CanvasContainer();
    ~CanvasContainer() {}
};
} // namespace interface