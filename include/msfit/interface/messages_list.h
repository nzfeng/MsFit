#pragma once

#include <gtkmm/frame.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>

class MessagesList : public Gtk::ScrolledWindow {
  public:
    MessagesList(const std::string& label, size_t maxMessages = 10);
    ~MessagesList() {}

    void setUpDialogPanel();

    class DialogColumns : public Gtk::TreeModel::ColumnRecord {
      public:
        DialogColumns() { add(m_col_text); }

        Gtk::TreeModelColumn<Glib::ustring> m_col_text;
    };

    DialogColumns dialogColumns;

    // Add a message to the dialog.
    void addMessageToList(const std::string& text);
    // Clear all messages.
    void clear();

  protected:
    Glib::RefPtr<Gtk::ListStore> m_refListStore; // The Tree Model.
    Gtk::TreeView m_TreeView;                    // The Tree View.

  private:
    std::string label;  // the title displayed in the window
    size_t maxMessages; // maximum number of messages to display in the dialog
};