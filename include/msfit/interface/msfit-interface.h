#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

namespace MsFitInterface {

class MainWindow : public Gtk::Window {

  public:
    MainWindow();
    ~MainWindow(){};
};

// Initialize gtkmm and enter the gtkmm main processing loop.
void init();

// === Global variables ===
namespace state {}

} // namespace MsFitInterface