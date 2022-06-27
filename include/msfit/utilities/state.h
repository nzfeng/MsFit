// Global variables

#pragma once

#include <array>
#include <map>
#include <string>

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
    extern int button_space;
    extern size_t maxFillOptions;
    extern size_t maxDialogMessages;
} // namespace params
} // namespace interface

namespace colors {
extern std::array<float, 3> white;
extern std::array<float, 3> black;
extern std::array<float, 3> gold;
extern std::array<float, 3> light_sky_blue;
extern std::array<float, 3> gray;
extern std::array<float, 3> light_gray;
extern std::array<float, 3> dark_gray;
extern std::array<float, 3> yellow_green;
extern std::array<float, 3> salmon;
extern std::array<float, 3> alice_blue;
extern std::array<float, 3> light_salmon;
extern std::array<float, 3> honeydew;
extern std::array<float, 3> papaya_whip;
} // namespace colors

namespace theme {

extern std::array<float, 3> color_outline;
extern std::array<float, 3> color_solid_outline; // outline of a solid cell
extern std::array<float, 3> color_solid;         // color of a solid cell (usually black)
extern std::array<float, 3> color_unselected;    // usually white
extern std::array<float, 3> color_selected;
extern std::array<float, 3> color_highlighted;
extern std::array<float, 3> color_word_highlight; // outline of a word
extern std::array<float, 3> color_pencil;
extern std::array<float, 3> color_pen;
extern std::array<float, 3> color_autofill;
extern std::array<float, 3> color_salmon;

extern float black_border_width;  // total width of the yellow border around a selected black square
extern float white_border_width;  // total width of the gray border around a white square
extern float puzzle_border_width; // width of border around the puzzle board
} // namespace theme

namespace cell {

enum state { UNSELECTED, SELECTED, HIGHLIGHTED };
enum utensil { PEN, PENCIL, AUTOFILL };

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
    extern int initRows; // initial # of rows, *not* necesssarily the current #
    extern int initCols; // initial # of cols, *not* necesssarily the current #
} // namespace params

} // namespace grid

namespace state {
extern bool makeSymmetric;
extern int symmetryMode;
extern bool enteringMultipleLetters;
extern bool pencilSelected;
extern bool lastCommandIsBackspace;
} // namespace state

// Stuff having to do with the word data
namespace data {
// Map from a pair of chars at the start of a word, to a regex pattern for the blank spot.
extern std::map<std::string, std::string> startingPairRegex;
// Map from a pair of chars at the end of a word, to a regex pattern for the blank spot.
extern std::map<std::string, std::string> endingPairRegex;
} // namespace data