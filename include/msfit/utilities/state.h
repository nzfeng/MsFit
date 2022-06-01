// Global variables

#pragma once

#include <array>

namespace interface {
namespace params {

    extern int margin;
    extern int window_width;
    extern int window_height;
    extern int grid_container_width;
    extern int grid_container_height;
    extern int right_menu_width;
    extern int bottom_menu_height;
    extern int separator_margin;
    extern int minButtonWidth;
    extern int minButtonHeight;
} // namespace params
} // namespace interface

namespace theme {
extern std::array<float, 3> color_unselected;
extern std::array<float, 3> color_selected;
extern std::array<float, 3> color_highlighted;
extern std::array<float, 3> color_salmon;

extern float black_border_width;  // total width of the yellow border around a selected black square
extern float white_border_width;  // total width of the gray border around a white square
extern float puzzle_border_width; // width of border around the puzzle board
} // namespace theme

namespace cell {

enum state { UNSELECTED, SELECTED, HIGHLIGHTED };

} // namespace cell

namespace grid {

namespace symmetry {
    enum state { TWO_TURN, ONE_TURN, MIRROR_UD, MIRROR_LR, ASYMMETRIC };
} // namespace symmetry

namespace wordtype {
    enum state { ACROSS, DOWN };
}

namespace params {
    extern int minRows;
    extern int maxRows;
    extern int N_ROWS; // initial # of rows
    extern int N_COLS; // initial # of cols
} // namespace params

} // namespace grid

namespace state {
extern bool makeSymmetric;
extern int symmetryMode;
extern bool enteringMultipleLetters;
extern bool pencilSelected;
} // namespace state