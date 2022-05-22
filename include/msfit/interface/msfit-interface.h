#pragma once

#include <gtkmm/application.h>

#include "msfit/interface/main-window.h"

namespace MsFitInterface {

// Forward declarations
class MainWindow;
class MainContainer;
class GridContainer;
class RightMenuContainer;
class BottomMenuContainer;

// Initialize gtkmm and enter the gtkmm main processing loop.
void init();

} // namespace MsFitInterface