#pragma once


#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
// #include <gtkmm/entry.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/notebook.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/switch.h>
#include <gtkmm/togglebutton.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "msfit/interface/messages_list.h"
#include "msfit/puzzle/puzzle_grid.h"

class RightMenuContainer : public Gtk::Notebook {

  public:
    RightMenuContainer();
    virtual ~RightMenuContainer();

    void setUpMenuPage();
    void setUpCluesPage();
    void setUpSummaryPage();

    // Frames containing user-defined callbacks, general crossword settings
    Gtk::Box menuBox, clueBox, summaryBox;
    Gtk::Frame userCallbacksFrame, settingsFrame;
    Gtk::Box userCallbacksBox, settingsBox;

    // Buttons
    // Gtk::CheckButton and Gtk::ToggleButton act as radio buttons, if they are included in a group.
    Gtk::ToggleButton pencilToggle;
    Gtk::Button clearGridButton;
    Gtk::Button loadAllWords;
    Gtk::Switch softConstraintToggle;
    Gtk::CheckButton constraintButtons[6];
    Glib::ustring constraintLabels[6] = {"No pop culture", "No proper nouns",    "No punctuation",
                                         "English-only",   "No partial phrases", "Root words"};
    Gtk::Button fillButtons[2];
    Glib::ustring fillButtonLabels[2] = {"Fill word", "Fill grid"};
    // Also display little scroll window showing the first n other matches when "fill word" is clicked.
    // TODO: When "Fill grid" is pressed, use this window to show different fill options (if any)
    MessagesList fillOptionsList;

    Gtk::CheckButton gridSymmetryButtons[4];
    Glib::ustring gridSymmetryLabels[4] = {"180°", "90°", "Mirror up-down", "Mirror left-right"};
    Gtk::Switch makeSymmetricButton, lockGridSize;

    Gtk::SpinButton gridDimSpin[2];
    Glib::ustring gridDimLabels[2] = {"Rows: ", "Cols: "};
    Gtk::CheckButton gridSizePresetButtons[4];
    int presetSizes[4] = {15, 21, 23, 25};
    Glib::ustring gridSizePresetLabels[4] = {"15 x 15", "21 x 21", "23 x 23", "25 x 25"};

    Gtk::Button saveButton, loadButton;

  private:
    // For Menu page -- grid settings
    Gtk::Grid setUpGridSymmetrySettings();
    Gtk::Grid setUpGridDimensionSettings();
    Gtk::Box setUpPuzzleIOSettings();
    // For Menu page -- user settings
    Gtk::Grid setUpWritingUtensilMenu();
    Gtk::Grid setUpWordCriteriaOptions();
    Gtk::Grid setUpWordlistOptions();
    Gtk::Grid setUpFillTools();

    // Signal handlers
    void on_makeSymmetric_button_toggled() const;
    void on_symmetry_button_clicked(int buttonIndex) const;

    void on_lockGrid_button_toggled();

    void on_pencil_button_clicked() const;
    void on_constraint_button_toggled() const;
};