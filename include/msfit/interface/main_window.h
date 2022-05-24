#pragma once

#include <gtkmm/paned.h>
#include <gtkmm/window.h>

#include "msfit/interface/bottom_menu_container.h"
#include "msfit/interface/canvas_container.h"
#include "msfit/interface/right_menu_container.h"
#include "msfit/puzzle/puzzle_grid.h"

class MainWindow : public Gtk::Window {

  public:
    MainWindow();
    virtual ~MainWindow() {}

  protected:
    // Container widgets:
    // 	- Gtk::Grid arranges its child widgets in rows and columns. Use attach() and attach_next_to() to insert child
    // widgets.
    //	- Gtk::Box arranges its child widgets vertically or horizontally. Use append() to insert child widgets.

    // Grid should be in an AspectFrame, whose aspect ratio will be constant no matter how the user resizes the
    // top-level window.
    //	- Each grid cell should be either a text box or a black square (which can perhaps be rendered using Cairo?)

    // Child widgets

    // Contains everything else
    Gtk::Paned mainContainer;
    // Contains the grid, and the bottom menu.
    Gtk::Paned gridContainer;
    // Contains the r.h.s menu.
    RightMenuContainer rightMenuContainer;
    // Contains the bottom menu/dialog.
    BottomMenuContainer bottomMenuContainer;
    // Contains the actual grid.
    CanvasContainer canvasContainer;
    // The actual Gtk::DrawingArea representing the puzzle.
    PuzzleGrid puzzleGrid;

    // TODO: Signal handlers
};