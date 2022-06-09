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