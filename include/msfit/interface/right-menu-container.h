#pragma once


#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/notebook.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinbutton.h>

// #include <fstream>
// #include <iostream>
// #include <sstream>

namespace MsFitInterface {

class RightMenuContainer : public Gtk::Notebook {

  public:
    RightMenuContainer();
    ~RightMenuContainer() {}

    void setUpMenuPage();
    void setUpCluesPage();
    Gtk::Grid setUpGridSymmetrySettings();
    Gtk::Grid setUpGridDimensionSettings();
    Gtk::Box setUpPuzzleIOSettings();

    // Frames containing user-defined callbacks, general crossword settings
    Gtk::Box menuBox, clueBox;
    Gtk::Frame userCallbacksFrame, settingsFrame;
    Gtk::Box userCallbacksBox, settingsBox;

    // Buttons
    // Gtk::CheckButton and Gtk::ToggleButton act as radio buttons, if they are included in a group.
    Gtk::CheckButton gridSymmetryButtons[4];
    Glib::ustring gridSymmetryLabels[4] = {"180°", "90°", "Mirror up-down", "Mirror left-right"};
    Gtk::SpinButton gridDimSpin[2];
    Glib::ustring gridDimLabels[2] = {"Rows: ", "Cols: "};
    Gtk::CheckButton gridSizePresetButtons[4];
    Glib::ustring gridSizePresetLabels[4] = {"15 x 15", "21 x 21", "23 x 23", "25 x 25"};

    Gtk::Button saveButton, loadButton;
};
} // namespace MsFitInterface