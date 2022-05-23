/*
 * A PuzzleGraph structure is used when the puzzle consists of more complicated cells besides squares. Then, it becomes
 * worthwhile to invest in a fancier data structure to enable navigation.
 */

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

namespace puzzle {

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
} // namespace puzzle