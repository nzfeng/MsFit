#include "msfit/interface/msfit-interface.h"

namespace MsFitInterface {
namespace state {

    // Default parameters for the interface (window sizes, etc.)
    int margin = 10;
    int window_width = 1200;
    int window_height = 800;
    int grid_container_width = 3 * window_width / 4;
    int grid_container_height = 3 * window_height / 4;
    int right_menu_width = window_width - grid_container_width;
    int bottom_menu_height = window_height - grid_container_height;
    int separator_margin = 5;
    int minButtonWidth = 40;
    int maxButtonWidth = 20;
    int minRows = 1;
    int maxRows = 1000;

} // namespace state
} // namespace MsFitInterface