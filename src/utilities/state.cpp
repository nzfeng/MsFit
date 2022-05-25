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
int minButtonHeight = 20;
int minRows = 1;
int maxRows = 1000;
int N_ROWS = 10;                              // the initial # of rows
int N_COLS = 15;                              // the initial # of cols
int grid_width = 0.9 * grid_container_height; // default size of the puzzle grid

} // namespace state

namespace theme {
float black_border_width = 2; // total width of the yellow border around a selected black square
float white_border_width = 1; // total width of the gray border around a white square
float puzzle_border_width = 2;
} // namespace theme