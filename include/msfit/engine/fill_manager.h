#pragma once

#include <string_view>

#include "msfit/engine/dataset_manager.h"
#include "msfit/interface/messages_list.h"
#include "msfit/puzzle/grid_word.h"
#include "msfit/puzzle/puzzle_grid.h"
#include "msfit/utilities/state.h"

class FillManager {
  public:
    FillManager(DatasetManager& datasetManager, PuzzleGrid& puzzleGrid);
    ~FillManager() {}

    std::vector<std::string> getWordFills(GridWord* word, bool ignorePenciled, const std::string& constraint = "none",
                                          bool printMessage = false, int nOptions = -1) const;

    GridWord* getMostConstrainedWord(bool ignorePenciled, const std::string& constraint);

    void fillGridDFS() const;

    // to send messages to the dialog
    MessagesList* bottomMenuContainer;

  private:
    DatasetManager& datasetManager;
    PuzzleGrid& puzzleGrid;

    bool doFillsExist(GridWord* word, bool ignorePenciled) const;
    std::regex getGridFeasibleRegex(GridWord* word, bool ignorePenciled) const;
    std::vector<std::string> getGridCompliantWords(GridWord* word, const std::vector<std::string>& matches,
                                                   bool ignorePenciled) const;

    void buildSearchStructures(std::vector<std::string>& cells, std::vector<std::vector<size_t>>& words,
                               std::vector<std::vector<size_t>>& xMap, std::vector<Square*>& cellToSquare) const;

    // TODO: All legal letter pairs...
};