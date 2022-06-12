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

    setUpMenuPage();
    setUpCluesPage();
    setUpSummaryPage();
}

RightMenuContainer::~RightMenuContainer() {}

void RightMenuContainer::setUpMenuPage() {

    // Set up the Menu page.
    int nSeps = 2;
    Gtk::Separator seps[2];
    for (int i = 0; i < nSeps; i++) {
        seps[i].set_margin_top(interface::params::separator_margin);
    }

    userCallbacksFrame.set_margin(interface::params::margin);
    userCallbacksFrame.set_label("User callbacks");
    userCallbacksBox.set_orientation(Gtk::Orientation::VERTICAL);

    Gtk::Grid pencilBox = setUpWritingUtensilMenu();
    // Gtk::Grid criteriaBox = setUpWordCriteriaOptions();
    Gtk::Grid wordlistBox = setUpWordlistOptions();
    Gtk::Grid fillBox = setUpFillTools();
    fillOptionsList.set_vexpand();
    // TODO: A button that evaluates the quality of the current highlighted word.

    userCallbacksBox.append(seps[0]);
    userCallbacksBox.append(pencilBox);
    // userCallbacksBox.append(criteriaBox);
    userCallbacksBox.append(wordlistBox);
    userCallbacksBox.append(fillBox);
    userCallbacksBox.append(fillOptionsList);
    userCallbacksFrame.set_child(userCallbacksBox);

    settingsFrame.set_margin(interface::params::margin);
    settingsFrame.set_label("Settings");
    settingsBox.set_orientation(Gtk::Orientation::VERTICAL);

    Gtk::Grid gridSymmetryContainer = setUpGridSymmetrySettings();
    Gtk::Grid gridSizeContainer = setUpGridDimensionSettings();
    Gtk::Box saveBox = setUpPuzzleIOSettings();

    // A Frame can only hold 1 item, so let that one item be a Box that contains everything else.
    settingsBox.append(seps[1]);
    settingsBox.append(gridSymmetryContainer);
    settingsBox.append(gridSizeContainer);
    settingsBox.append(saveBox);
    settingsFrame.set_child(settingsBox);

    // A page of the Notebook can only be 1 item; let it be <menuBox>.
    menuBox.set_orientation(Gtk::Orientation::VERTICAL);
    menuBox.set_spacing(interface::params::margin); // min number of pixels between each widget
    menuBox.append(userCallbacksFrame);
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
        gridSizeBox.attach(gridSizePresetButtons[i], col, row + 2);
        if (i > 0) gridSizePresetButtons[i].set_group(gridSizePresetButtons[0]);
        if (grid::params::initRows == presetSizes[i] && grid::params::initCols == presetSizes[i]) {
            gridSizePresetButtons[i].set_active(true);
        }
    }
    return gridSizeBox;
}

Gtk::Box RightMenuContainer::setUpPuzzleIOSettings() {

    // TODO: Add icons. See https://developer-old.gnome.org/gtkmm-tutorial/stable/sec-pushbuttons.html.en
    // TODO: Add option to export to .puz
    Gtk::Box saveBox;
    saveBox.set_spacing(interface::params::button_space);
    saveBox.set_orientation(Gtk::Orientation::HORIZONTAL);
    saveBox.set_margin(interface::params::margin);
    saveBox.set_homogeneous(true);
    saveBox.set_spacing(interface::params::margin);
    saveButton.set_label("Save");
    // saveButton.set_image_from_icon_name("info");
    loadButton.set_label("Load");
    saveBox.append(saveButton);
    saveBox.append(loadButton);
    return saveBox;
}

Gtk::Grid RightMenuContainer::setUpWritingUtensilMenu() {
    Gtk::Grid pencilBox;
    pencilBox.set_row_spacing(interface::params::button_space);
    pencilBox.set_column_spacing(interface::params::button_space);
    pencilBox.set_margin(interface::params::margin);
    pencilBox.set_orientation(Gtk::Orientation::HORIZONTAL);
    pencilBox.set_row_homogeneous(true);
    pencilBox.set_row_spacing(0);
    pencilToggle.set_label("Pencil");
    pencilToggle.set_halign(Gtk::Align::END);
    Gtk::Image pencilIcon("../../data/icons/pencil-icon.png");
    pencilToggle.set_child(pencilIcon);
    pencilToggle.signal_toggled().connect(sigc::mem_fun(*this, &RightMenuContainer::on_pencil_button_clicked));
    pencilBox.attach(pencilToggle, 0, 0);
    return pencilBox;
}

Gtk::Grid RightMenuContainer::setUpWordCriteriaOptions() {
    Gtk::Grid criteriaBox;
    criteriaBox.set_margin(interface::params::margin);
    criteriaBox.set_column_homogeneous(false);
    Gtk::Label criteriaLabel("Word criteria: ");
    criteriaLabel.set_halign(Gtk::Align::START);
    criteriaBox.attach(criteriaLabel, 0, 0);

    softConstraintToggle.set_name("Soft constraints");
    softConstraintToggle.set_halign(Gtk::Align::END);
    softConstraintToggle.set_active(false);
    // softConstraintToggle.property_active().signal_changed().connect(
    //     sigc::mem_fun(*this, &RightMenuContainer::on_constraint_button_toggled));
    criteriaBox.attach(softConstraintToggle, 1, 0);

    int nConstraints = *(&constraintButtons + 1) - constraintButtons;
    int nRows = 2;
    int nCols = 2;
    // int width = right_menu_width / 3; // int height;
    for (int i = 0; i < nConstraints; i++) {
        int row = i / nCols;
        int col = i - row * nCols;
        constraintButtons[i].set_label(constraintLabels[i]);
        // constraintButtons[i].signal_toggled().connect(
        //     sigc::bind(sigc::mem_fun(*this, &RightMenuContainer::on_criterion_button_clicked), i));
        criteriaBox.attach(constraintButtons[i], col, row + 1);
        constraintButtons[i].set_active(true);
    }

    return criteriaBox;
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

    return wordlistBox;
}

Gtk::Grid RightMenuContainer::setUpFillTools() {
    Gtk::Grid fillBox;
    fillBox.set_row_spacing(interface::params::button_space);
    fillBox.set_column_spacing(interface::params::button_space);
    fillBox.set_margin(interface::params::margin);
    fillBox.set_column_homogeneous(false);
    Gtk::Label label("Fill: ");
    label.set_halign(Gtk::Align::START);
    fillBox.attach(label, 0, 0);

    int nOptions = *(&fillButtons + 1) - fillButtons;
    for (int i = 0; i < nOptions; i++) {
        fillButtons[i].set_label(fillButtonLabels[i]);
        fillBox.attach(fillButtons[i], i, 1);
    }

    return fillBox;
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


void RightMenuContainer::on_makeSymmetric_button_toggled() const {
    state::makeSymmetric = makeSymmetricButton.get_active();
}

/*
 * On of the preset symmetry options was clicked.
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