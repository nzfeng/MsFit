#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

class DatasetManager {
  public:
    DatasetManager() {}
    ~DatasetManager() {}

    void loadData();

    // datasets, indexed by wordlength
    std::map<size_t, std::vector<std::string>> words;

  private:
    std::string dir = "../../data";     // hard-coded for now
    std::string binFile = dir + "/bin"; // hard-coded for now

    // Load in datasets
    void loadFromFile();
    void createBinary();
    void loadBinary();

    // Sort datasets -- split into groups based on word length.

    // Store stats on the dataset (distribution of word lengths, letter placement, # of occurences, etc.)
};