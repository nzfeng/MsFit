#pragma once

#include <string_view>

#include "msfit/engine/dataset_manager.h"
#include "msfit/puzzle/grid_word.h"

class FillManager {
  public:
    FillManager(DatasetManager& datasetManager);
    ~FillManager() {}

    std::vector<std::string> getWordFills(GridWord* word, std::string& message, size_t nOptions = 10);

  private:
    DatasetManager& datasetManager;
};