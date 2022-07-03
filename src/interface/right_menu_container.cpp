#include "msfit/interface/right_menu_container.h"
#include "msfit/utilities/state.h"
#include <gtkmm/image.h>

/*
 * Construct r.h.s. menu. This menu will have multiple tabs:
 *
 *	(1) Menu/settings (general crossword settings)
 *	(2) Clues
 */
RightMenuContainer::RightMenuContainer() : fillOptionsList("Options", interface::params::maxFillOptions) {

    set_margin(interface::params::margin);
    set_expand();
}

RightMenuContainer::~RightMenuContainer() {}

/*
 * Have a separate function outside of the constructor, because some of the signal handlers depend on member variables
 * that have yet to be defined (PuzzleGrid, etc.)
 */
void RightMenuContainer::setup(PuzzleGrid& puzzleGrid_, FillManager& fillManager_) {

    puzzleGrid = &puzzleGrid_;
    fillManager = &fillManager_;
    setUpMenuPage();
    setUpCluesPage();
    setUpSummaryPage(); // TODO: Set up signal handler; when tab is clicked, update stats.
}

void RightMenuContainer::setUpMenuPage() {

    // Set up the Menu page.
    int nSeps = 2;
    Gtk::Separator seps[2];
    for (int i = 0; i < nSeps; i++) {
        seps[i].set_margin_top(interface::params::separator_margin);
    }

    fillOptionsFrame.set_margin(interface::params::margin);
    fillOptionsFrame.set_label("Fill tools");
    fillOptionsBox.set_orientation(Gtk::Orientation::VERTICAL);

    Gtk::Grid toolbar = setUpToolbar();
    Gtk::Grid fillBox = setUpFillTools();
    fillOptionsList.set_vexpand();
    // TODO: A button that evaluates the quality of the current selected word.

    fillOptionsBox.append(seps[0]);
    fillOptionsBox.append(toolbar);
    fillOptionsBox.append(fillBox);
    fillOptionsBox.append(fillOptionsList);
    fillOptionsFrame.set_child(fillOptionsBox);

    settingsFrame.set_margin(interface::params::margin);
    settingsFrame.set_label("Settings");
    settingsBox.set_orientation(Gtk::Orientation::VERTICAL);

    Gtk::Grid gridSymmetryContainer = setUpGridSymmetrySettings();
    Gtk::Grid gridSizeContainer = setUpGridDimensionSettings();

    // A Frame can only hold 1 item, so let that one item be a Box that contains everything else.
    settingsBox.append(seps[1]);
    settingsBox.append(gridSymmetryContainer);
    settingsBox.append(gridSizeContainer);
    settingsFrame.set_child(settingsBox);

    // A page of the Notebook can only be 1 item; let it be <menuBox>.
    menuBox.set_orientation(Gtk::Orientation::VERTICAL);
    menuBox.set_spacing(interface::params::margin); // min number of pixels between each widget
    menuBox.append(fillOptionsFrame);
    menuBox.append(settingsFrame);
    append_page(menuBox, "Menu");
}

Gtk::Grid RightMenuContainer::setUpGridSymmetrySettings() {

    Gtk::Grid gridSymmetryBox;
    gridSymmetryBox.set_margin(interface::params::margin);
    Gtk::Label gridSymmetryLabel("Grid symmetry: ");
    gridSymmetryLabel.set_halign(Gtk::Align::START);
    gridSymmetryBox.attach(gridSymmetryLabel, 0, 0);

    makeSymmetricButton.set_name("Symmetry toggle");
    makeSymmetricButton.set_halign(Gtk::Align::END);
    makeSymmetricButton.set_active(state::makeSymmetric);
    makeSymmetricButton.set_can_focus(false);
    makeSymmetricButton.property_active().signal_changed().connect(
        sigc::mem_fun(*this, &RightMenuContainer::on_makeSymmetric_button_toggled));
    gridSymmetryBox.attach(makeSymmetricButton, 1, 0);

    // TODO: Idk how to label a switch

    int nGridSymmetries = *(&gridSymmetryButtons + 1) - gridSymmetryButtons;
    int nRows = 2;
    int nCols = 2;
    for (int i = 0; i < nGridSymmetries; i++) {
        int row = i / nCols;
        int col = i - row * nCols;
        gridSymmetryButtons[i].set_label(gridSymmetryLabels[i]);
        gridSymmetryButtons[i].set_can_focus(false);
        gridSymmetryButtons[i].signal_toggled().connect(
            sigc::bind(sigc::mem_fun(*this, &RightMenuContainer::on_symmetry_button_clicked), i));
        gridSymmetryBox.attach(gridSymmetryButtons[i], col, row + 1);
        if (i > 0) gridSymmetryButtons[i].set_group(gridSymmetryButtons[0]);
        gridSymmetryButtons[i].set_active(i == state::symmetryMode);
    }
    return gridSymmetryBox;
}

Gtk::Grid RightMenuContainer::setUpGridDimensionSettings() {

    Gtk::Grid gridSizeBox;
    gridSizeBox.set_column_homogeneous(true);
    // gridSizeBox.set_column_spacing(20);
    gridSizeBox.set_margin(interface::params::margin);
    Gtk::Label gridSizeLabel("Grid dimensions: ");
    gridSizeLabel.set_halign(Gtk::Align::START);
    gridSizeBox.attach(gridSizeLabel, 0, 0);

    lockGridSize.set_name("Lock size");
    lockGridSize.set_halign(Gtk::Align::END);
    lockGridSize.set_can_focus(false);
    lockGridSize.set_active(false);
    lockGridSize.property_active().signal_changed().connect(
        sigc::mem_fun(*this, &RightMenuContainer::on_lockGrid_button_toggled));
    gridSizeBox.attach(lockGridSize, 1, 0);
    // TODO: Idk how to label a switch

    // spin buttons
    int nDims = *(&gridDimLabels + 1) - gridDimLabels;
    for (int i = 0; i < nDims; i++) {
        gridDimSpin[i].set_editable(true);
        gridDimSpin[i].set_numeric(true); // prevent user from typing non-numeric chars
        gridDimSpin[i].set_range(grid::params::minRows, grid::params::maxRows);
        gridDimSpin[i].set_increments(1, 1); // step, page (left/right mouse clicks)
        gridDimSpin[i].set_wrap();
        // gridDimSpin[i].set_climb_rate(); // accleration rate when button is held down
        gridDimSpin[i].set_snap_to_ticks(true);
        gridDimSpin[i].set_size_request(interface::params::minButtonWidth, interface::params::minButtonHeight);
        gridDimSpin[i].signal_value_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &RightMenuContainer::on_sizeSpinner_clicked), i));
        gridSizeBox.attach(gridDimSpin[i], i, 1);
    }
    gridDimSpin[0].set_value(grid::params::initRows);
    gridDimSpin[1].set_value(grid::params::initCols);

    // preset sizes
    int nGridSizes = *(&gridSizePresetButtons + 1) - gridSizePresetButtons;
    int nRows = 2;
    int nCols = 2;
    for (int i = 0; i < nGridSizes; i++) {
        int row = i / nCols;
        int col = i - row * nCols;
        gridSizePresetButtons[i].set_label(gridSizePresetLabels[i]);
        gridSizePresetButtons[i].set_can_focus(false);
        gridSizePresetButtons[i].signal_toggled().connect(
            sigc::bind(sigc::mem_fun(*this, &RightMenuContainer::on_size_button_clicked), i));
        gridSizeBox.attach(gridSizePresetButtons[i], col, row + 2);
        if (i > 0) gridSizePresetButtons[i].set_group(gridSizePresetButtons[0]);
        if (grid::params::initRows == presetSizes[i] && grid::params::initCols == presetSizes[i]) {
            gridSizePresetButtons[i].set_active(true);
        }
    }
    return gridSizeBox;
}

Gtk::Grid RightMenuContainer::setUpToolbar() {
    Gtk::Grid toolbar;
    toolbar.set_row_spacing(interface::params::button_space);
    toolbar.set_column_spacing(interface::params::button_space);
    toolbar.set_margin(interface::params::margin);
    toolbar.set_orientation(Gtk::Orientation::HORIZONTAL);
    toolbar.set_row_homogeneous(true);
    toolbar.set_row_spacing(0);

    // // TODO: Set up Save, Load, Export buttons. I don't think there will be enough options that a full drop-down File
    // // menu is warranted. Similarly, options for loading in wordlists -> dropdown in the RHS menu.
    // saveButton.set_label("Save"); saveButton.set_can_focus(false); Gtk::Image
    // saveIcon("../../data/icons/save-icon.png"); saveButton.set_child(saveIcon);
    // // loadButton.signal_clicked().connect(sigc::mem_fun(*this, &RightMenuContainer::on_save_button_clicked));
    // toolbar.attach(saveButton, 0, 0);

    // loadButton.set_label("Load");
    // loadButton.set_can_focus(false);
    // Gtk::Image loadIcon("../../data/icons/load-icon.png");
    // loadButton.set_child(loadIcon);
    // // loadButton.signal_clicked().connect(sigc::mem_fun(*this, &RightMenuContainer::on_load_button_clicked));
    // toolbar.attach(loadButton, 0, 0);

    pencilToggle.set_label("Pencil");
    // pencilToggle.set_halign(Gtk::Align::END);
    pencilToggle.set_can_focus(false);
    Gtk::Image pencilIcon("../../data/icons/pencil-icon.png");
    pencilToggle.set_child(pencilIcon);
    pencilToggle.signal_toggled().connect(sigc::mem_fun(*this, &RightMenuContainer::on_pencil_button_clicked));
    toolbar.attach(pencilToggle, 0, 0);

    clearGridButton.set_label("Clear");
    clearGridButton.set_can_focus(false);
    Gtk::Image clearIcon("../../data/icons/trash-icon.png");
    clearGridButton.set_child(clearIcon);
    clearGridButton.signal_clicked().connect(sigc::mem_fun(*this, &RightMenuContainer::on_clearGrid_button_clicked));
    toolbar.attach(clearGridButton, 1, 0);
    return toolbar;
}

Gtk::Grid RightMenuContainer::setUpWordlistOptions() {
    Gtk::Grid wordlistBox;
    wordlistBox.set_row_spacing(interface::params::button_space);
    wordlistBox.set_column_spacing(interface::params::button_space);
    wordlistBox.set_margin(interface::params::margin);
    wordlistBox.set_column_homogeneous(false);
    Gtk::Label label("Load wordlists: ");
    label.set_halign(Gtk::Align::START);
    wordlistBox.attach(label, 0, 0);

    wordlistBox.attach(loadAllWords, 0, 1);
    loadAllWords.set_label("Load all");
    loadAllWords.set_can_focus(false);

    return wordlistBox;
}

Gtk::Grid RightMenuContainer::setUpFillTools() {
    Gtk::Grid fillBox;
    fillBox.set_row_spacing(interface::params::button_space);
    fillBox.set_column_spacing(interface::params::button_space);
    fillBox.set_margin(interface::params::margin);
    fillBox.set_column_homogeneous(false);

    // Add checkbox that lets user choose whether autofill ignores penciled-in cells.
    ignorePenciled.set_label("Ignore penciled");
    ignorePenciled.set_can_focus(false);
    fillBox.attach(ignorePenciled, 0, 0);

    int nOptions = *(&fillButtons + 1) - fillButtons;
    for (int i = 0; i < nOptions; i++) {
        fillButtons[i].setText(fillButtonLabels[i]);
        fillButtons[i].set_can_focus(false);
        fillButtons[i].signal_clicked().connect(
            sigc::bind(sigc::mem_fun(*this, &RightMenuContainer::on_fill_clicked), fillButtonLabels[i]));
    }

    // Attach "Fill word"
    fillBox.attach(fillButtons[0], 0, 0);
    int nFillConstraints = *(&fillConstraints + 1) - fillConstraints;
    int nCols = 2;
    int nRows = nFillConstraints / nCols;
    // Add radio buttons that allow user to specify whether they want to limit fills of the current word to only those
    // that are "grid-feasible" or "grid-compliant", i.e. fills that don't lead to other words being unfillable.
    for (int i = 0; i < nFillConstraints; i++) {
        fillConstraints[i].set_label(fillConstraintLabels[i]);
        fillConstraints[i].set_can_focus(false);
        int row = i / nCols;
        int col = i - row * nCols;
        fillBox.attach(fillConstraints[i], col, row + 2);
        if (i > 0) fillConstraints[i].set_group(fillConstraints[0]);
    }
    fillConstraints[0].set_active(true);

    // Attach "getMostConstrainedButton"
    getMostConstrainedButton.setText("get most constrained");
    getMostConstrainedButton.setFontSize(10);
    getMostConstrainedButton.set_can_focus(false);
    getMostConstrainedButton.signal_clicked().connect(
        sigc::mem_fun(*this, &RightMenuContainer::on_getMostConstrained_clicked));
    fillBox.attach(getMostConstrainedButton, 1, 0);

    // Attach "Fill grid"
    fillBox.attach(fillButtons[1], 0, nRows + 3);

    return fillBox;
}

std::string RightMenuContainer::getFillWordConstraint() const {
    int nFillConstraints = *(&fillConstraints + 1) - fillConstraints;
    for (int i = 0; i < nFillConstraints; i++) {
        if (fillConstraints[i].get_active()) return fillConstraintLabels[i];
    }
    return ""; // shouldn't get here
}

void RightMenuContainer::setUpCluesPage() { append_page(clueBox, "Clues"); }

void RightMenuContainer::setUpSummaryPage() {

    // TODO
    Gtk::Grid summaryGrid;
    summaryGrid.set_row_spacing(interface::params::button_space);
    summaryGrid.set_column_spacing(interface::params::button_space);
    summaryGrid.set_margin(interface::params::margin);

    auto numWhiteSquaresLabel = Gtk::make_managed<Gtk::Label>("Number of white squares: ");
    auto numBlackSquaresLabel = Gtk::make_managed<Gtk::Label>("Number of black squares: ");
    auto numWordsLabel = Gtk::make_managed<Gtk::Label>("Number of words: ");
    auto numBlocksLabel = Gtk::make_managed<Gtk::Label>("Number of blocks: ");
    // TODO: Diversity of letters, syllables/phonemes in the puzzle.

    auto numWhiteSquares = Gtk::make_managed<Gtk::Label>(std::to_string(0));
    auto numBlackSquares = Gtk::make_managed<Gtk::Label>(std::to_string(0));
    auto numWords = Gtk::make_managed<Gtk::Label>(std::to_string(0));
    auto numBlocks = Gtk::make_managed<Gtk::Label>(std::to_string(0));

    std::vector<Gtk::Label*> labels = {numWhiteSquaresLabel, numBlackSquaresLabel, numWordsLabel, numBlocksLabel};
    std::vector<Gtk::Label*> values = {numWhiteSquares, numBlackSquares, numWords, numBlocks};
    for (size_t i = 0; i < labels.size(); i++) {
        labels[i]->set_halign(Gtk::Align::START);
        values[i]->set_halign(Gtk::Align::END);
        summaryGrid.attach(*labels[i], 0, i);
        summaryGrid.attach(*values[i], 1, i);
    }

    summaryBox.append(summaryGrid);
    append_page(summaryBox, "Summary");
}


// =================================== SIGNAL HANDLERS ===================================

void RightMenuContainer::on_size_button_clicked(int buttonIndex) {

    size_t nRows = presetSizes[buttonIndex];
    size_t nCols = nRows;
    // Update values in the spin buttons to reflect the change
    gridDimSpin[0].set_value(nRows);
    gridDimSpin[1].set_value(nCols);

    puzzleGrid->setSize(nRows, nCols);
}

void RightMenuContainer::on_sizeSpinner_clicked(int buttonIndex) {
    // Assume there are only 2 dimensions.
    if (buttonIndex == 0) {
        puzzleGrid->setRows(gridDimSpin[buttonIndex].get_value_as_int());
        return;
    }
    puzzleGrid->setCols(gridDimSpin[buttonIndex].get_value_as_int());
}


void RightMenuContainer::generate_word_fills() {
    std::vector<std::string> fills = fillManager->getWordFills(
        puzzleGrid->getSelectedWord(), ignorePenciled.get_active(), getFillWordConstraint(), true, -1);

    // Populate the little scroll window with fills
    fillOptionsList.clear();
    // instead of forcing the TreeView to continually add/delete options
    if (fills.size() > interface::params::maxFillOptions) fills.resize(interface::params::maxFillOptions);
    for (auto fill : fills) {
        fillOptionsList.addMessageToList(fill);
    }
}

void RightMenuContainer::on_fill_clicked(const std::string& button) {

    if (button == "Fill word") {
        generate_word_fills();
    } else if (button == "Fill grid") {
        // TODO
    }
}

void RightMenuContainer::on_clearGrid_button_clicked() {
    puzzleGrid->clear(); // TODO: Set message "Puzzle cleared"?
}

void RightMenuContainer::on_makeSymmetric_button_toggled() const {
    state::makeSymmetric = makeSymmetricButton.get_active();
}

/*
 * One of the preset symmetry options was clicked.
 */
void RightMenuContainer::on_symmetry_button_clicked(int buttonIndex) const {
    // selected button = gridSymmetryButtons[buttonIndex]
    state::symmetryMode = buttonIndex; // assuming buttonIndex == symmetryMode
}

/*
 * Grey-out the size buttons, indicating that current grid size is locked.
 */
void RightMenuContainer::on_lockGrid_button_toggled() {

    // spin buttons
    int nDims = *(&gridDimLabels + 1) - gridDimLabels;
    for (int i = 0; i < nDims; i++) {
        gridDimSpin[i].set_sensitive(!lockGridSize.get_active());
    }

    // preset sizes
    int nGridSizes = *(&gridSizePresetButtons + 1) - gridSizePresetButtons;
    for (int i = 0; i < nGridSizes; i++) {
        gridSizePresetButtons[i].set_sensitive(!lockGridSize.get_active());
    }
}

void RightMenuContainer::on_pencil_button_clicked() const { state::pencilSelected = pencilToggle.get_active(); }

void RightMenuContainer::on_getMostConstrained_clicked() {
    GridWord* word = fillManager->getMostConstrainedWord(ignorePenciled.get_active(), getFillWordConstraint());
    puzzleGrid->setHighlightedWord(word);
}