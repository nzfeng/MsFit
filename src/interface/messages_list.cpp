#include "msfit/interface/messages_list.h"
#include "msfit/utilities/state.h"

#include <fstream>
#include <iostream>
#include <sstream>

/*
 * Constructor
 */
MessagesList::MessagesList(const std::string& label_, size_t maxMessages_) : label(label_), maxMessages(maxMessages_) {

    set_margin(interface::params::margin);
    setUpDialogPanel();
}

MessagesList::~MessagesList() {}

DialogColumns::~DialogColumns() {}

/*
 * Set up the dialog window.
 */
void MessagesList::setUpDialogPanel() {

    // Create a new scrolled window, with scrollbars only if needed
    set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    set_child(m_TreeView);
    // create list store
    m_refListStore = Gtk::ListStore::create(dialogColumns);
    m_TreeView.set_model(m_refListStore);

    // Add the Model's column to the View's columns
    m_TreeView.append_column(label, dialogColumns.m_col_text);
}

/*
 * Add a message to the dialog.
 */
void MessagesList::addMessageToList(const std::string& text) {

    auto row = *(m_refListStore->append());
    row[dialogColumns.m_col_text] = text;

    auto start = m_refListStore->children().begin();
    if (m_refListStore->children().size() > maxMessages) m_refListStore->erase(start);
}

void MessagesList::clear() {

    auto children = m_refListStore->children();
    // Repeatedly get and delete pointer to the first element.
    // while (m_refListStore->children().size() > 0) {
    //     m_refListStore->erase(m_refListStore->children().begin());
    // }

    auto it = children.begin();
    auto end = children.end();
    while (it != end) {
        it = m_refListStore->erase(it); // erase should auto-increment iterator
    }
}