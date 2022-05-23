#include "msfit/interface/canvas_container.h"
#include "msfit/utilities/state.h"

namespace interface {

/*
 * Construct canvas container that contains the actual board. Contained in the grid container.
 */
CanvasContainer::CanvasContainer() {

    set_margin(state::margin);
    set_xalign(Gtk::Align::CENTER);
    set_yalign(Gtk::Align::CENTER);
    set_obey_child(false);
    set_ratio(1.0);

    // Configure actual grid
}

} // namespace interface