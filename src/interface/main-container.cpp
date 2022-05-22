#include "msfit/interface/main-container.h"
#include "msfit/interface/state.h"

namespace MsFitInterface {


MainContainer::MainContainer() {

    set_margin(state::margin);
    set_orientation(Gtk::Orientation::HORIZONTAL);
    set_position(state::grid_container_width); // intiial position of slider between grid and r.h.s. menu
}

} // namespace MsFitInterface