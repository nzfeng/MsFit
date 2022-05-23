#include "msfit/interface/right_menu_container.h"
#include "msfit/utilities/state.h"

namespace interface {

/*
 * Construct r.h.s. menu. This menu will have multiple tabs:
 *
 *	(1) Menu/settings (general crossword settings)
 *	(2) Clues
 */
RightMenuContainer::RightMenuContainer() {
    set_margin(state::margin);
    // set_expand();

    setUpMenuPage();
    setUpCluesPage();
}

void RightMenuContainer::setUpMenuPage() {

    // Set up the Menu page.
    int nSeps = 2;
    Gtk::Separator seps[2];
    for (int i = 0; i < nSeps; i++) {
        seps[i].set_margin_top(state::separator_margin);
    }

    userCallbacksFrame.set_margin(state::margin);
    userCallbacksFrame.set_label("User callbacks");
    userCallbacksBox.set_orientation(Gtk::Orientation::VERTICAL);
    userCallbacksBox.append(seps[0]);

    settingsFrame.set_margin(state::margin);
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
    menuBox.set_spacing(state::margin); // min number of pixels between each widget
    menuBox.append(userCallbacksFrame);
    menuBox.append(settingsFrame);
    append_page(menuBox, "Menu");
}

Gtk::Grid RightMenuContainer::setUpGridSymmetrySettings() {

    Gtk::Grid gridSymmetryBox;
    gridSymmetryBox.set_margin(state::margin);
    Gtk::Label gridSymmetryLabel("Grid symmetry: ");
    gridSymmetryLabel.set_halign(Gtk::Align::START);
    gridSymmetryBox.attach(gridSymmetryLabel, 0, 0);

    int nGridSymmetries = *(&gridSymmetryButtons + 1) - gridSymmetryButtons;
    int nRows = 2;
    int nCols = 2;
    for (int i = 0; i < nGridSymmetries; i++) {
        int row = i / nCols;
        int col = i - row * nCols;
        gridSymmetryButtons[i].set_label(gridSymmetryLabels[i]);
        gridSymmetryBox.attach(gridSymmetryButtons[i], col, row + 1);
        if (i > 0) gridSymmetryButtons[i].set_group(gridSymmetryButtons[0]);
    }
    gridSymmetryButtons[0].set_active(true);
    return gridSymmetryBox;
}

Gtk::Grid RightMenuContainer::setUpGridDimensionSettings() {

    Gtk::Grid gridSizeBox;
    gridSizeBox.set_column_homogeneous(true);
    // gridSizeBox.set_column_spacing(20);
    gridSizeBox.set_margin(state::margin);
    Gtk::Label gridSizeLabel("Grid dimensions: ");
    gridSizeLabel.set_halign(Gtk::Align::START);
    gridSizeBox.attach(gridSizeLabel, 0, 0);

    // spin buttons
    int nDims = *(&gridDimLabels + 1) - gridDimLabels;
    for (int i = 0; i < nDims; i++) {
        gridDimSpin[i].set_numeric(false);
        gridDimSpin[i].set_range(state::minRows, state::maxRows);
        gridDimSpin[i].set_increments(1, 1); // step, page (left/right mouse clicks)
        // gridDimSpin[i].set_climb_rate(); // accleration rate when button is held down
        gridDimSpin[i].set_snap_to_ticks(true);
        gridDimSpin[i].set_size_request(state::minButtonWidth, state::minButtonHeight);
        gridSizeBox.attach(gridDimSpin[i], i, 1);
    }
    gridDimSpin[0].set_value(state::N_ROWS);
    gridDimSpin[1].set_value(state::N_COLS);

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
    }
    gridSizePresetButtons[0].set_active(true);
    return gridSizeBox;
}

Gtk::Box RightMenuContainer::setUpPuzzleIOSettings() {

    // TODO: Add icons. See https://developer-old.gnome.org/gtkmm-tutorial/stable/sec-pushbuttons.html.en
    Gtk::Box saveBox;
    saveBox.set_orientation(Gtk::Orientation::HORIZONTAL);
    saveBox.set_margin(state::margin);
    saveBox.set_homogeneous(true);
    saveBox.set_spacing(state::margin);
    saveButton.set_label("Save");
    // saveButton.set_image_from_icon_name("info");
    loadButton.set_label("Load");
    saveBox.append(saveButton);
    saveBox.append(loadButton);
    return saveBox;
}

void RightMenuContainer::setUpCluesPage() { append_page(clueBox, "Clues"); }

} // namespace interface