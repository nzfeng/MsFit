#include "msfit/interface/main_window.h"
#include "msfit/utilities/state.h"
#include <gtkmm/eventcontrollerkey.h>

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
    : rightMenuContainer(), bottomMenuContainer(), canvasContainer(),
      puzzleGrid(grid::params::initRows, grid::params::initCols) {

    set_title("MsFit");
    set_default_size(interface::params::window_width, interface::params::window_height);

    // Paradigm: Each individual constructor handles all the settings (margin size, default size,
    // orientation, function callbacks, etc.) All the parenting actions are done here, in one place, so that the
    // whole hierarchy (parent-child relationships) can be seen at a glance, as well as all of the signal handling.

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

    connectMenuButtons();

    canvasContainer.set_margin(interface::params::margin);
    canvasContainer.set_xalign(Gtk::Align::CENTER);
    canvasContainer.set_yalign(Gtk::Align::CENTER);
    canvasContainer.set_obey_child(false);
    canvasContainer.set_ratio(1.0); // aspect ratio of grid is maintained
    canvasContainer.set_child(puzzleGrid);

    auto keyHandler = Gtk::EventControllerKey::create();
    keyHandler->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_key_press), "bubble"),
                                             false);
    add_controller(keyHandler);

    keyHandler = Gtk::EventControllerKey::create();
    keyHandler->set_propagation_phase(Gtk::PropagationPhase::TARGET);
    keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_key_press), "target"),
                                             false);
    add_controller(keyHandler);

    keyHandler = Gtk::EventControllerKey::create();
    keyHandler->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_key_press), "capture"),
                                             false);
    add_controller(keyHandler);
}


/*
 * Set up all the buttons in the RHS menu that need access to the other members of the MainWindow.
 */
void MainWindow::connectMenuButtons() {

    // Attach the size buttons in RHS menu here (ideally would be entirely contained within RightMenuContainer class,
    // but we need access to PuzzleGrid to dynamically update the grid size.)
    int nGridSizes = *(&rightMenuContainer.gridSizePresetButtons + 1) - rightMenuContainer.gridSizePresetButtons;
    for (int i = 0; i < nGridSizes; i++) {
        rightMenuContainer.gridSizePresetButtons[i].signal_toggled().connect(
            sigc::bind(sigc::mem_fun(*this, &MainWindow::on_size_button_clicked), i));
    }
    int nDims = *(&rightMenuContainer.gridDimLabels + 1) - rightMenuContainer.gridDimLabels;
    for (int i = 0; i < nDims; i++) {
        rightMenuContainer.gridDimSpin[i].signal_value_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &MainWindow::on_sizeSpinner_clicked), i));
    }
}

// =================================== SIGNAL HANDLERS ===================================

/*
 * Overwrite default window key handler.
 */
bool MainWindow::on_key_press(guint keyval, guint keycode, Gdk::ModifierType state, const Glib::ustring& phase) {

    // std::cerr << "Phase: " << phase << std::endl;

    // std::cerr << has_focus() << std::endl;
    // std::cerr << "mainContainer " << mainContainer.has_focus() << std::endl;
    // std::cerr << "gridContainer " << gridContainer.has_focus() << std::endl;
    // std::cerr << "rhs menu " << rightMenuContainer.has_focus() << std::endl;
    // std::cerr << "bottom menu " << bottomMenuContainer.has_focus() << std::endl;
    // std::cerr << "canvasContainer " << canvasContainer.has_focus() << std::endl;
    // std::cerr << "puzzleGrid " << puzzleGrid.has_focus() << std::endl;

    if (phase == "capture" && !puzzleGrid.has_focus()) {
        state::lastCommandIsBackspace = false; // TODO: set this variable upon any mouse click as well
        return true;
    }
    return false;
}

/*
 * On of the preset grid sizes in the RHS menu was clicked. Ideally, this would have remained in the RightMenuContainer
 * class, but we need access to puzzleGrid.
 */
void MainWindow::on_size_button_clicked(int buttonIndex) {

    size_t nRows = rightMenuContainer.presetSizes[buttonIndex];
    size_t nCols = nRows;
    // Update values in the spin buttons to reflect the change
    rightMenuContainer.gridDimSpin[0].set_value(nRows);
    rightMenuContainer.gridDimSpin[1].set_value(nCols);

    puzzleGrid.setSize(nRows, nCols);
}

/*
 * On of the grid size spinners in the RHS menu was clicked. Ideally, this would have remained in the RightMenuContainer
 * class, but we need access to puzzleGrid.
 */
void MainWindow::on_sizeSpinner_clicked(int buttonIndex) {
    // Assume there are only 2 dimensions.
    if (buttonIndex == 0) {
        puzzleGrid.setRows(rightMenuContainer.gridDimSpin[buttonIndex].get_value_as_int());
        return;
    }
    puzzleGrid.setCols(rightMenuContainer.gridDimSpin[buttonIndex].get_value_as_int());
}
