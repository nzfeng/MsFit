// #include <array>
#include "msfit/utilities/state.h"

namespace interface {
namespace params {
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
    int minButtonHeight = 20;
    // int grid_width = 0.9 * grid_container_height; // default size of the puzzle grid
} // namespace params
} // namespace interface

namespace grid {
namespace params {
    int minRows = 1;
    int maxRows = 1000;
    int initRows = 10; // the initial # of rows
    int initCols = 15; // the initial # of cols
} // namespace params
} // namespace grid

namespace theme {
std::array<float, 3> color_unselected = {1.0, 1.0, 1.0};                 // white
std::array<float, 3> color_selected = {1.0, 0.843137, 0.0};              // gold
std::array<float, 3> color_highlighted = {0.690196, 0.866667, 0.984314}; // light sky blue, desaturated
std::array<float, 3> color_pen = {0, 0, 0};                              // black
std::array<float, 3> color_pencil = {0.6, 0.6, 0.6};                     // gray
std::array<float, 3> color_autofill = {0.603922, 0.803922, 0.196078};    // yellow-green
std::array<float, 3> color_salmon = {0.98039, 0.50196, 4453125};         // salmon

float black_border_width = 2; // total width of the yellow border around a selected black square
float white_border_width = 1; // total width of the gray border around a white square
float puzzle_border_width = 2;
} // namespace theme

namespace state {
bool makeSymmetric = true;
int symmetryMode = grid::symmetry::TWO_TURN;
bool enteringMultipleLetters = false;
bool pencilSelected = false;
bool lastCommandIsBackspace = false;
} // namespace state