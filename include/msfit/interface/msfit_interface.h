#pragma once

#include <gtkmm/application.h>

#include "msfit/interface/main_window.h"

namespace interface {

// Forward declarations
class MainWindow;
class MainContainer;
class GridContainer;
class RightMenuContainer;
class BottomMenuContainer;

// Initialize gtkmm and enter the gtkmm main processing loop.
void init();

} // namespace interface