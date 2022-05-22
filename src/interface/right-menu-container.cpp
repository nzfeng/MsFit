#include "msfit/interface/right-menu-container.h"

namespace MsFitInterface {

/*
 * Construct r.h.s. menu. This menu will have multiple tabs:
 *
 *	(1) Menu/settings (general crossword settings)
 *	(2) Clues
 */
RightMenuContainer::RightMenuContainer() {
    set_margin(margin);
    set_expand();
    append_page(label_1, "Menu");
    append_page(label_2, "Clues");
    // signal_switch_page().connect(sigc::mem_fun(*this, &ExampleWindow::on_notebook_switch_page));
}

} // namespace MsFitInterface