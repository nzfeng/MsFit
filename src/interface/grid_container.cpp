#include "msfit/interface/grid_container.h"
#include "msfit/utilities/state.h"

namespace interface {

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

} // namespace interface