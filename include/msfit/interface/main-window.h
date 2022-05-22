#pragma once

#include "gtkmm/window.h"

#include "msfit/interface/bottom-menu-container.h"
#include "msfit/interface/canvas-container.h"
#include "msfit/interface/grid-container.h"
#include "msfit/interface/main-container.h"
#include "msfit/interface/right-menu-container.h"

namespace MsFitInterface {

class MainWindow : public Gtk::Window {

  public:
    MainWindow();
    ~MainWindow() {}

  protected:
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


    // Contains everything else
    MainContainer mainContainer;

    // Contains the grid, and the bottom menu.
    GridContainer gridContainer;

    // Contains the r.h.s menu.
    RightMenuContainer rightMenuContainer;

    // Contains the bottom menu/dialog.
    BottomMenuContainer bottomMenuContainer;

    // Contains the actual grid.
    CanvasContainer canvasContainer;

    // TODO: Signal handlers
};

} // namespace MsFitInterface