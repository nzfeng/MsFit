#pragma once

#include <gtkmm/paned.h>

#include "msfit/interface/right_menu_container.h"

namespace interface {

class MainContainer : public Gtk::Paned {

  public:
    MainContainer();
    ~MainContainer() {}
};
} // namespace interface