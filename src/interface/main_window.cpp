#include "msfit/interface/main_window.h"
#include "msfit/utilities/state.h"

#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gestureclick.h>

/*
 * Construct main window, and auto-initialize its components.
 *
 * <mainContainer> = The immediate child of <MainWindow>. Contains everything else.
 * <rightMenuContainer> = Contains the menu on the r.h.s. The main menu panel.
 * <gridContainer> = Contains the "active" components: the grid and the bottom dialog.
 * <canvasContainer> = Contains the actual grid (crossword).
 * <bottomMenuContainer> = Containst the bottom dialog.
 */
MainWindow::MainWindow()
    : rightMenuContainer(), bottomMenuContainer("Dialog", interface::params::maxDialogMessages, true),
      canvasContainer(), puzzleGrid(grid::params::initRows, grid::params::initCols), datasetManager(),
      fillManager(datasetManager, puzzleGrid) {

    set_title("MsFit");
    set_default_size(interface::params::window_width, interface::params::window_height);

    // Pass around all the pointers/reference variables we need to enable info-passing between different objects
    rightMenuContainer.setup(puzzleGrid, fillManager);

    mainContainer.set_margin(interface::params::margin);
    mainContainer.set_orientation(Gtk::Orientation::HORIZONTAL);
    mainContainer.set_position(interface::params::grid_container_width); // intial position of slider

    gridContainer.set_margin(interface::params::margin);
    gridContainer.set_orientation(Gtk::Orientation::VERTICAL);
    gridContainer.set_position(interface::params::grid_container_height);

    // Add the paned widget to the top-level window.
    set_child(mainContainer);
    // Add the contents of the two halves of the paned widget.
    mainContainer.set_start_child(gridContainer);
    mainContainer.set_end_child(rightMenuContainer);

    gridContainer.set_start_child(canvasContainer);
    gridContainer.set_end_child(bottomMenuContainer);

    puzzleGrid.grab_focus(); // makes puzzleGrid have the keyboard focus
    bottomMenuContainer.set_can_focus(false);

    canvasContainer.set_margin(interface::params::margin);
    canvasContainer.set_xalign(Gtk::Align::CENTER);
    canvasContainer.set_yalign(Gtk::Align::CENTER);
    canvasContainer.set_obey_child(false);
    canvasContainer.set_ratio(1.0); // aspect ratio of grid is maintained
    canvasContainer.set_child(puzzleGrid);

    // Pass around all the pointers/reference variables we need to enable info-passing between different objects
    fillManager.bottomMenuContainer = &bottomMenuContainer;
    datasetManager.bottomMenuContainer = &bottomMenuContainer;
    // puzzleGrid.mainWindow = this;

    // Load wordlist as default upon startup.
    loadData();
}

// =================================== SIGNAL HANDLERS ===================================

void MainWindow::loadData() { datasetManager.loadData(); }

MainWindow::~MainWindow() {}