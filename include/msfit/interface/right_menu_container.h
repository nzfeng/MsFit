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

#include "msfit/engine/fill_manager.h"
#include "msfit/interface/messages_list.h"
#include "msfit/interface/msfit_button.h"
#include "msfit/puzzle/puzzle_grid.h"

class RightMenuContainer : public Gtk::Notebook {

  public:
    RightMenuContainer();
    virtual ~RightMenuContainer();

    void setup(PuzzleGrid& puzzleGrid, FillManager& fillManager);
    void updateSummaryStats();

    // These need to be pointers rather than reference variables, because this object will never be constructed with
    // these variables to auto-initialize. In fact, none of the main objects in MainWindow can contain references to
    // other objects, because they all contain circular references to each other.
    PuzzleGrid* puzzleGrid;
    FillManager* fillManager;

  private:
    // Frames containing user-defined callbacks, general crossword settings
    Gtk::Box menuBox, clueBox, summaryBox;
    Gtk::Frame fillOptionsFrame, settingsFrame;
    Gtk::Box fillOptionsBox, settingsBox;

    // Buttons
    // Gtk::CheckButton and Gtk::ToggleButton act as radio buttons, if they are included in a group.
    Gtk::ToggleButton pencilToggle;
    Gtk::Button clearGridButton, saveButton, loadButton;
    Gtk::Button loadAllWords;
    MsFitButton fillButtons[2];
    Glib::ustring fillButtonLabels[2] = {"Fill word", "Fill grid"};
    MsFitButton getMostConstrainedButton;
    Gtk::CheckButton ignorePenciled;
    Gtk::CheckButton fillConstraints[3];
    Glib::ustring fillConstraintLabels[3] = {"Grid-feasible", "Grid-compliant", "None"};
    std::string getFillWordConstraint() const;
    // Also display little scroll window showing the first n other matches when "fill word" is clicked.
    // TODO: Options for sorting fill options by various criteria (alphabetical, various fill-quality metrics [diversity
    // of letters, etc.])
    MessagesList fillOptionsList;

    Gtk::CheckButton gridSymmetryButtons[4];
    Glib::ustring gridSymmetryLabels[4] = {"180°", "90°", "Mirror up-down", "Mirror left-right"};
    Gtk::Switch makeSymmetricButton, lockGridSize;

    Gtk::SpinButton gridDimSpin[2];
    Glib::ustring gridDimLabels[2] = {"Rows: ", "Cols: "};
    Gtk::CheckButton gridSizePresetButtons[4];
    int presetSizes[4] = {15, 21, 23, 25};
    Glib::ustring gridSizePresetLabels[4] = {"15 x 15", "21 x 21", "23 x 23", "25 x 25"};

    Gtk::Label numWhiteSquares, numBlackSquares, numWords;

    void setUpMenuPage();
    void setUpCluesPage();
    void setUpSummaryPage(size_t nWhiteSquares, size_t nBlackSquares, size_t nWords);

    // For Menu page -- grid settings
    Gtk::Grid setUpGridSymmetrySettings();
    Gtk::Grid setUpGridDimensionSettings();
    // For Menu page -- user settings
    Gtk::Grid setUpToolbar();
    Gtk::Grid setUpWordlistOptions();
    Gtk::Grid setUpFillTools();

    // Signal handlers
    void on_size_button_clicked(int buttonIndex);
    void on_sizeSpinner_clicked(int buttonIndex);

    void generate_word_fills();
    void on_fill_clicked(const std::string& button);

    void on_clearGrid_button_clicked();

    void on_makeSymmetric_button_toggled() const;
    void on_symmetry_button_clicked(int buttonIndex) const;

    void on_lockGrid_button_toggled();

    void on_pencil_button_clicked() const;
    void on_constraint_button_toggled() const;

    void on_getMostConstrained_clicked();
};