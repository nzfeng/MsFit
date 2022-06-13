#pragma once

#include <string_view>

#include "msfit/engine/dataset_manager.h"
#include "msfit/puzzle/grid_word.h"
#include "msfit/puzzle/puzzle_grid.h"

class FillManager {
  public:
    FillManager(DatasetManager& datasetManager);
    ~FillManager() {}

    std::vector<std::string> getWordFills(GridWord* word, std::string& message, int nOptions = 10) const;
    void fillGridDFS(PuzzleGrid& puzzleGrid, std::string& message) const;

  private:
    DatasetManager& datasetManager;
};
