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
      puzzleGrid(grid::params::N_ROWS, grid::params::N_COLS) {

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

    // keyHandler = Gtk::EventControllerKey::create();
    // keyHandler->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    // keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "bubble"),
    //                                          false);
    // puzzleGrid.add_controller(keyHandler);

    // keyHandler = Gtk::EventControllerKey::create();
    // keyHandler->set_propagation_phase(Gtk::PropagationPhase::TARGET);
    // keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "target"),
    //                                          false);
    // puzzleGrid.add_controller(keyHandler);

    // keyHandler = Gtk::EventControllerKey::create();
    // keyHandler->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    // keyHandler->signal_key_pressed().connect(sigc::bind(sigc::mem_fun(*this, &PuzzleGrid::on_key_press), "capture"),
    //                                          false);
    // puzzleGrid.add_controller(keyHandler);
}

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
        return true;
    }
    return false;
}
