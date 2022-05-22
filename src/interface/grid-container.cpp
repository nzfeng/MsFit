#include "msfit/interface/grid-container.h"
#include "msfit/interface/state.h"

namespace MsFitInterface {

/*
 * Construct grid container. This is another Paned object with two panes:
 *
 * (1) The grid area
 * (2) The bottom menu/dialog
 */
GridContainer::GridContainer() {
    set_margin(state::margin);
    set_orientation(Gtk::Orientation::VERTICAL);
    set_position(state::grid_container_height); // initial position of slider between canvas and bottom menu container
}

} // namespace MsFitInterface