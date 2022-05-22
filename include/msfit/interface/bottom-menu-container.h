#pragma once

#include "gtkmm/frame.h"

namespace MsFitInterface {

class BottomMenuContainer : public Gtk::Frame {
  public:
    BottomMenuContainer();
    ~BottomMenuContainer() {}

    int margin = 10;
};
} // namespace MsFitInterface