#include "msfit/interface/grid-container.h"

namespace MsFitInterface {

/*
 * Construct grid container. This is another Paned object with two panes:
 *
 * (1) The grid area
 * (2) The bottom menu/dialog
 */
GridContainer::GridContainer() : bottomMenuContainer() {
    set_margin(margin);
    set_orientation(Gtk::Orientation::VERTICAL);
}

} // namespace MsFitInterface