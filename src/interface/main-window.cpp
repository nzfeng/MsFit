#include "msfit/interface/main-window.h"
#include "msfit/interface/state.h"

namespace MsFitInterface {
/*
 * Construct main window, and auto-initialize its components.
 *
 * <mainContainer> = The immediate child of <MainWindow>. Contains everything else.
 * <rightMenuContainer> = Contains the menu on the r.h.s. The main menu panel.
 * <gridContainer> = Contains the "active" components: the grid and the bottom dialog.
 * <canvasContainer> = Contains the actual grid (crossword).
 * <bottomMenuContainer> = Containst the bottom dialog.
 */
MainWindow::MainWindow()
    : mainContainer(), gridContainer(), rightMenuContainer(), bottomMenuContainer(), canvasContainer() {

    set_title("MsFit");
    set_default_size(state::window_width, state::window_height);

    // Paradigm: Each individual constructor handles all the settings (margin size, default size,
    // orientation, more complicated setups, etc.) All the parenting actions are done here, in one place, so that the
    // whole hierarchy (parent-child relationships) can be seen at a glance.

    // TODO: It may be overkill to define a separate class for each container widget; the application may end up being
    // simple enough that we can just declare/define everything here.

    // Add the paned widget to the top-level window.
    set_child(mainContainer);
    // Add the contents of the two halves of the paned widget.
    mainContainer.set_start_child(gridContainer);
    mainContainer.set_end_child(rightMenuContainer);

    gridContainer.set_start_child(canvasContainer);
    gridContainer.set_end_child(bottomMenuContainer);
}
} // namespace MsFitInterface
