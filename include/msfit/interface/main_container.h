#pragma once

#include <gtkmm/paned.h>

#include "msfit/interface/right_menu_container.h"

class MainContainer : public Gtk::Paned {

  public:
    MainContainer();
    virtual ~MainContainer() {}
};