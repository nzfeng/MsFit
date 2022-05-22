#include "msfit/interface/bottom-menu-container.h"
#include "msfit/interface/state.h"

namespace MsFitInterface {

/*
 * Construct bottom menu. Contained in the grid container.
 */
BottomMenuContainer::BottomMenuContainer() {
    set_margin(state::margin);
    set_expand();
}

} // namespace MsFitInterface