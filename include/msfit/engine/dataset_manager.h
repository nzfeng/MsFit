#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "msfit/interface/messages_list.h"
#include "msfit/utilities/state.h"

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

// TODO: Currently passing messages to the message dialog in the bottom menu isn't very elegant. Would probably make
// more sense if everything was defined in the main_window.cpp file (which contains the central logic of the program),
// but currently I like the class setup I have currently, for modularity and code maintenance reasons... I may need to
// just keep programming for a while to evolve towards better practices.

typedef std::unordered_map<std::string, std::set<size_t>> HashMap;

class DatasetManager {
  public:
    DatasetManager();
    ~DatasetManager() {}

    void loadData();

    // datasets, indexed by wordlength
    // TODO: better to make this private, and define get() since it should not be altered by any other object
    std::map<size_t, std::vector<std::string>> words;
    // // Map from a pair of chars at the start of a word, to a regex pattern for the blank spot.
    // std::map<std::string, std::string> startingPairRegex = {{".", "."}};
    // // Map from a pair of chars at the end of a word, to a regex pattern for the blank spot.
    // std::map<std::string, std::string> endingPairRegex = {{".", "."}};

    // Hash maps (one for each word length) that map possible queries to a list of results.
    // TODO: Better to make this private and define get()
    // word length => hash map
    // hash value => set of indices into datasetManager.words[n]
    void buildHashTables();
    std::unordered_map<size_t, HashMap> hashMaps;

    MessagesList* bottomMenuContainer;

  private:
    std::string dir = "../../data";     // hard-coded for now
    std::string binFile = dir + "/bin"; // hard-coded for now

    std::set<std::string> ALL_LETTERS = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                                         "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

    // Load in datasets
    void loadFromFile();
    void createBinary();
    void loadBinary();

    // Sort datasets -- split into groups based on word length.

    // Store stats on the dataset (distribution of word lengths, letter placement, # of occurences, etc.)
    std::vector<std::string> getUnusedLetterPairs(const std::string& location) const;
    std::map<std::string, std::string> unusedPairsToRegexMap(const std::vector<std::string>& unusedPairs) const;
    void analyzeLetterPairs() const;
};