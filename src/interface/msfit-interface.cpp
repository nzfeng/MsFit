#include "msfit/interface/msfit-interface.h"

namespace MsFitInterface {

MainWindow::MainWindow() {
    set_title("MsFit");
    set_default_size(800, 600);
}

void init() {
    // Create a Gtk::Application object, stored in a Glib::RefPtr smartpointer. This is needed in all gtkmm
    // applications. The create() method initializes gtkmm.
    auto app = Gtk::Application::create("");

    // Enter the gtkmm main processing loop, which will finish when the window is closed.
    app->make_window_and_run<MainWindow>(0, nullptr);
}


} // namespace MsFitInterface