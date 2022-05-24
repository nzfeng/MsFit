// Global variables

#pragma once

namespace state {

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
extern int minRows;
extern int maxRows;
extern int N_ROWS; // initial # of rows
extern int N_COLS; // initial # of cols
extern int grid_width;

} // namespace state

namespace cell {

enum state { UNSELECTED, SELECTED, HIGHLIGHTED };

} // namespace cell

// namespace theme {
// extern Vector3 UNSELECTED;
// extern Vector3 SELECTED;
// extern Vector3 HIGHLIGHTED;
// } // namespace theme