#include "msfit/interface/main-window.h"

namespace MsFitInterface {
/*
 * Construct main window, and auto-initialize its immediate child, the main container (which is a Gtk::Paned object.)
 */
MainWindow::MainWindow() : mainContainer(), gridContainer(), rightMenuContainer() {

    set_title("MsFit");
    set_default_size(window_width, window_height);
    mainContainer.set_margin(margin);
    // Add the paned widget to the top-level window.
    set_child(mainContainer);
    // Add the contents of the two halves of the paned widget.
    mainContainer.set_start_child(gridContainer);
    mainContainer.set_end_child(rightMenuContainer);
    // The children of <mainContainer> set their own children in their own constructors.

    // TODO: Set up rest of the window here, for ease-of-understanding. Easier to understand the hierarchy if it's all
    // here and can be seen at a glance.
}
} // namespace MsFitInterface
