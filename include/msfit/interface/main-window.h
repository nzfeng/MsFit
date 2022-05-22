#pragma once

#include "gtkmm/window.h"

#include "msfit/interface/grid-container.h"
#include "msfit/interface/main-container.h"

namespace MsFitInterface {

class MainWindow : public Gtk::Window {

  public:
    MainWindow();
    ~MainWindow() {}

  protected:
    // Defaults
    int window_width = 800;
    int window_height = 600;
    int margin = 10;

    // TODO: Make containers for menu panels
    // Child widgets

    // Container widgets:
    // 	- Gtk::Grid arranges its child widgets in rows and columns. Use attach() and attach_next_to() to insert child
    // widgets.
    //	- Gtk::Box arranges its child widgets vertically or horizontally. Use append() to insert child widgets.

    // Grid should be in an AspectFrame, whose aspect ratio will be constant no matter how the user resizes the
    // top-level window.
    //	- Each grid cell should be either a text box or a black square (which can perhaps be rendered using Cairo?)

    // Use Gtk::make_managed() when adding children to container widgets, so children automatically get destroyed when
    // its parent is destroyed (See "Memory Management")


    // Container widget splitting the main window into the grid area, and r.h.s. menu panel.
    // Will contain two children: (1) container widget for grid area, and (2) a container widget for r.h.s. menu panel.
    // The container widget for the grid area will itself contain two containers: (1) the grid; (2) the bottom
    // panel/dialog.
    MainContainer mainContainer;

    // Contains the grid, and the bottom menu.
    GridContainer gridContainer;

    // Contains the r.h.s menu.
    RightMenuContainer rightMenuContainer;

    // TODO: Signal handlers
};
} // namespace MsFitInterface