#include "msfit/interface/bottom_menu_container.h"
#include "msfit/utilities/state.h"

/*
 * Construct bottom menu. Contained in the grid container.
 */
BottomMenuContainer::BottomMenuContainer() {
    // set_margin(interface::params::margin);
    setUpDialogPanel();
}

/*
 * Set up the dialog window.
 */
void BottomMenuContainer::setUpDialogPanel() {

    // Create a new scrolled window, with scrollbars only if needed
    set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    set_child(m_TreeView);
    // create list store
    m_refListStore = Gtk::ListStore::create(dialogColumns);
    m_TreeView.set_model(m_refListStore);

    // Add some dummy messages for now
    for (int i = 0; i < 5; ++i) {
        std::ostringstream text;
        text << "message #" << i;

        auto row = *(m_refListStore->append());
        row[dialogColumns.m_col_text] = text.str();
    }

    // Add the Model's column to the View's columns
    m_TreeView.append_column("Dialog", dialogColumns.m_col_text);
}