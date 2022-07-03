/*
 * The main window is the "central" object where the program comes together.
 * All objects used in the program (frontend & backend) are contained here.
 */

#pragma once

#include <gtkmm/aspectframe.h>
#include <gtkmm/paned.h>
#include <gtkmm/window.h>

#include "msfit/engine/dataset_manager.h"
#include "msfit/engine/fill_manager.h"
#include "msfit/interface/messages_list.h"
#include "msfit/interface/right_menu_container.h"
#include "msfit/puzzle/puzzle_grid.h"

class MainWindow : public Gtk::Window {

  public:
    MainWindow();
    virtual ~MainWindow();

  private:
    /* Child widgets */

    // Contains everything else
    Gtk::Paned mainContainer;
    // Contains the grid, and the bottom menu.
    Gtk::Paned gridContainer;
    // Contains the r.h.s menu.
    RightMenuContainer rightMenuContainer;
    // Contains the bottom menu/dialog.
    MessagesList bottomMenuContainer;
    // Contains the actual grid.
    Gtk::AspectFrame canvasContainer;
    // The actual Gtk::DrawingArea representing the puzzle.
    PuzzleGrid puzzleGrid;

    /* Backend */
    DatasetManager datasetManager;
    FillManager fillManager;

    void loadData();
    void generate_word_fills();
    void on_fill_clicked(const std::string& button);
};