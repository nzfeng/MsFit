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
int N_ROWS = 15;                              // the initial # of rows
int N_COLS = 15;                              // the initial # of cols
int grid_width = 0.9 * grid_container_height; // default size of the puzzle grid

} // namespace state

// namespace theme {
// Vector3 UNSELECTED = {1.0, 1.0, 1.0};  // white
// Vector3 SELECTED = {0.0, 0.0, 0.0};    // pale yellow
// Vector3 HIGHLIGHTED = {0.0, 0.0, 1.0}; // pale blue
// } // namespace theme