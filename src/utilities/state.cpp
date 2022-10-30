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
    int button_space = 5;
    // int grid_width = 0.9 * grid_container_height; // default size of the puzzle grid
    size_t maxFillOptions = 50000;  // maximum number of fill options to display
    size_t maxDialogMessages = 100; // max number of messages in the bottom dialog to keep
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

namespace colors {
std::array<float, 3> white = {1.0, 1.0, 1.0};
std::array<float, 3> black = {0.0, 0.0, 0.0};
std::array<float, 3> gold = {1.0, 0.843137, 0.0};
std::array<float, 3> light_sky_blue = {0.690196, 0.866667, 0.984314}; // light sky blue, desaturated
std::array<float, 3> gray = {0.6, 0.6, 0.6};
std::array<float, 3> light_gray = {0.8, 0.8, 0.8};
std::array<float, 3> dark_gray = {0.2, 0.2, 0.2};
std::array<float, 3> yellow_green = {0.603922, 0.803922, 0.196078};
std::array<float, 3> salmon = {0.98039, 0.50196, 4453125};
std::array<float, 3> alice_blue = {0.941176, 0.972549, 1.};
std::array<float, 3> light_salmon = {1., 0.627451, 0.478431};
std::array<float, 3> honeydew = {0.941176, 1., 0.941176};
std::array<float, 3> papaya_whip = {1., 0.937255, 0.835294};
} // namespace colors

namespace theme {
std::array<float, 3> color_outline = colors::light_gray;
std::array<float, 3> color_solid_outline = colors::dark_gray;
std::array<float, 3> color_solid = colors::black;
std::array<float, 3> color_unselected = colors::white;
std::array<float, 3> color_selected = colors::gold;
std::array<float, 3> color_highlighted = colors::light_sky_blue;
std::array<float, 3> color_word_highlight = colors::light_salmon;
std::array<float, 3> color_pen = colors::black;
std::array<float, 3> color_pencil = colors::gray;
std::array<float, 3> color_autofill = colors::yellow_green;

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

namespace data {
// Map from a pair of chars at the start of a word, to a regex pattern for the blank spot.
std::map<std::string, std::string> startingPairRegex;
// Map from a pair of chars at the end of a word, to a regex pattern for the blank spot.
std::map<std::string, std::string> endingPairRegex;
} // namespace data