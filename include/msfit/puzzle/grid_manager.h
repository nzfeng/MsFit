/*
 * Functions for grid layout.
 */

#pragma once

#include "msfit/puzzle/puzzle_grid.h"

class GridManager {

  public:
    GridManager(PuzzleGrid& puzzleGrid);
    ~GridManager() {}

    // Determine the current symmetry of the grid. Returns the enum corresponding to the symmetry (see state.h)
    int isSymmetric() const;

    // TODO: Auto-generate some symmetric layouts. White squares must form a single connected component.

  private:
    PuzzleGrid& puzzleGrid;
};