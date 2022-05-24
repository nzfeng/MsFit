#pragma once

#include <gtkmm/frame.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>

class BottomMenuContainer : public Gtk::ScrolledWindow {
  public:
    BottomMenuContainer();
    virtual ~BottomMenuContainer() {}

    void setUpDialogPanel();

    class DialogColumns : public Gtk::TreeModel::ColumnRecord {
      public:
        DialogColumns() { add(m_col_text); }

        Gtk::TreeModelColumn<Glib::ustring> m_col_text;
    };

    DialogColumns dialogColumns;

  protected:
    Glib::RefPtr<Gtk::ListStore> m_refListStore; // The Tree Model.
    Gtk::TreeView m_TreeView;                    // The Tree View.
};