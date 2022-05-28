#include "msfit/puzzle/grid_manager.h"
#include "msfit/utilities/state.h"

GridManager::GridManager(PuzzleGrid& puzzleGrid_) : puzzleGrid(puzzleGrid_) {}

int GridManager::isSymmetric() const {
    size_t nRows = puzzleGrid.nRows();
    size_t nCols = puzzleGrid.nCols();

    if (puzzleGrid.isTwoTurnSymmetric()) return grid::symmetry::TWO_TURN;
    if (puzzleGrid.isOneTurnSymmetric()) return grid::symmetry::ONE_TURN;
    if (puzzleGrid.isMirroredUpDown()) return grid::symmetry::MIRROR_UD;
    if (puzzleGrid.isMirroredLeftRight()) return grid::symmetry::MIRROR_LR;

    return grid::symmetry::ASYMMETRIC;
}

/*
 * Auto-generate a layout, with the given symmetry. Rules:
 *	- White squares must form a single connected component.
 * 	- Blocks must be at least 3 squares in height/width
 */
// void GridManager::generateLayout(int symmetry) {

//     switch (symmetry) {
//     case grid::symmetry::TWO_TURN:
//         break;
//     }
// }