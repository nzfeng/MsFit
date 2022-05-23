#pragma once

#include <gtkmm/paned.h>

#include "msfit/interface/right-menu-container.h"

namespace MsFitInterface {

class MainContainer : public Gtk::Paned {

  public:
    MainContainer();
    ~MainContainer() {}
};
} // namespace MsFitInterface