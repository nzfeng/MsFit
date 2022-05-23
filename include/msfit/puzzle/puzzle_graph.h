#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace engine {

class PuzzleGraph {
  public:
    PuzzleGraph();
    ~PuzzleGraph();

    void savePuzzle();
    void loadPuzzle();

  private:
    // TODO: Arrays containing all the elements and navigation data


    // Based on the current pattern of white/black squares, determine the words of the puzzle (contiguous blocks of
    // white squares) and their numbers.
    void determineWords();
};
} // namespace engine