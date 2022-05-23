#include "msfit/interface/main_container.h"
#include "msfit/interface/state.h"

namespace interface {


MainContainer::MainContainer() {

    set_margin(state::margin);
    set_orientation(Gtk::Orientation::HORIZONTAL);
    set_position(state::grid_container_width); // intiial position of slider between grid and r.h.s. menu
}

} // namespace interface