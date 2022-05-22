#include "msfit/interface/msfit-interface.h"

namespace MsFitInterface {
namespace state {
    // Default parameters for the interface (window sizes, etc.)
    int margin = 10;
    int window_width = 800;
    int window_height = 600;
    int grid_container_width = 3 * window_width / 4;
    int grid_container_height = 3 * window_height / 4;
    int right_menu_width = window_width - grid_container_width;
    int bottom_menu_height = window_height - grid_container_height;

} // namespace state
} // namespace MsFitInterface