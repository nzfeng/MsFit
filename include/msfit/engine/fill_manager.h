#pragma once

#include <string_view>

#include "msfit/engine/dataset_manager.h"
#include "msfit/puzzle/grid_word.h"

class FillManager {
  public:
    FillManager(DatasetManager& datasetManager);
    ~FillManager() {}

    std::vector<std::string> getWordFills(GridWord* word, int nOptions = 10);

  private:
    DatasetManager& datasetManager;
};