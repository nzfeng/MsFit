#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// TODO: Currently passing messages to the message dialog in the bottom menu isn't very elegant. Would probably make
// more sense if everything was defined in the main_window.cpp file (which contains the central logic of the program),
// but currently I like the class setup I have currently, for modularity and code maintenance reasons... I may need to
// just keep programming for a while to evolve towards better practices.

class DatasetManager {
  public:
    DatasetManager();
    ~DatasetManager() {}

    void loadData(std::string& message);

    // datasets, indexed by wordlength
    std::map<size_t, std::vector<std::string>> words;
    // // Map from a pair of chars at the start of a word, to a regex pattern for the blank spot.
    // std::map<std::string, std::string> startingPairRegex = {{".", "."}};
    // // Map from a pair of chars at the end of a word, to a regex pattern for the blank spot.
    // std::map<std::string, std::string> endingPairRegex = {{".", "."}};

  private:
    std::string dir = "../../data";     // hard-coded for now
    std::string binFile = dir + "/bin"; // hard-coded for now

    std::set<std::string> ALL_LETTERS = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                                         "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

    // Load in datasets
    void loadFromFile(std::string& message);
    void createBinary(std::string& message);
    void loadBinary(std::string& message);

    // Sort datasets -- split into groups based on word length.

    // Store stats on the dataset (distribution of word lengths, letter placement, # of occurences, etc.)
    std::vector<std::string> getUnusedLetterPairs(const std::string& location) const;
    std::map<std::string, std::string> unusedPairsToRegexMap(const std::vector<std::string>& unusedPairs) const;
    void analyzeLetterPairs() const;
};