#include "msfit/engine/dataset_manager.h"

DatasetManager::DatasetManager() {}

/*
 * Load in all the wordlists from crossword-dataset.
 * TODO: Add options to GUI to specify which wordlists to load in (may be a far-future option.)
 */
void DatasetManager::loadFromFile() {

    // Get all txt files in the data directory
    std::string path, num, fn;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        path = entry.path();

        // Determine the length of words in this file.
        fn = path.substr(0, path.find_last_of(".")); // strip the ".txt"
        num = fn.substr(fn.find_last_of("_") + 1);
        std::stringstream sstream(num);
        size_t n;
        sstream >> n;

        std::vector<std::string> words_n;
        std::ifstream file(path.c_str());
        std::string line;
        if (file.is_open()) {
            while (!file.eof()) {
                getline(file, line);
                if (line == "") continue;
                words_n.push_back(line);
            }
            file.close();
            words.emplace(n, words_n);
        } else {
            std::cerr << "Could not open file <" << path << ">!" << std::endl;
        }
    }
    if (words.size() > 0) {
        bottomMenuContainer->addMessageToList("Data loaded from file.");
    }
}

/*
 * Create a binary file from the txt wordlist.
 */
void DatasetManager::createBinary() {}


/*
 * Read from a saved binary file containing the entire wordlist.
 */
void DatasetManager::loadBinary() {}

void DatasetManager::loadData() {
    if (words.size() > 0) {
        bottomMenuContainer->addMessageToList("Data already loaded.");
        return;
    }

    std::ifstream file(binFile.c_str());
    if (!file.is_open()) {
        loadFromFile();
    }
    // TODO: else load from binary

    // Run stats
    analyzeLetterPairs();
}

/*
 * Helper function for analyzeLetterPairs().
 *
 * Out of all 26^2 possible two-letter combinations, return the set of pairs that *don't* appear at the
 * [beginning | middle | end] of any word in the dataset.
 *
 * TODO: Maybe have some threshold for how many times a pair can appear before it counts a legal.
 * For example, the presence of acronyms in the dataset messes up this method. Maybe don't compare to
 * the dataset; compare to an English dictionary (which should only include full words, and few acronyms.)
 */
std::vector<std::string> DatasetManager::getUnusedLetterPairs(const std::string& location) const {

    // Generate all letter pairs
    std::vector<std::string> unusedPairs;
    for (const auto& l1 : ALL_LETTERS) {
        for (const auto& l2 : ALL_LETTERS) {
            unusedPairs.push_back(l1 + l2);
        }
    }

    for (auto const& [n, words_n] : words) {
        for (auto const& word : words_n) {
            std::string pair; // TODO: Change wordlist to std::string_views?
            if (location == "beginning") {
                pair = word.substr(0, 2);
            } else if (location == "end") {
                pair = word.substr(word.size() - 2);
            }
            unusedPairs.erase(std::remove(unusedPairs.begin(), unusedPairs.end(), pair), unusedPairs.end());
        }
    }
    return unusedPairs;
}

/*
 * Helper function for analyzeLetterPairs().
 *
 * Takes in a std::vector<std::string> of unused letter pairs, and generates the regex map.
 */
std::map<std::string, std::string>
DatasetManager::unusedPairsToRegexMap(const std::vector<std::string>& unusedPairs) const {

    // Construct map from pair patterns -> list of chars that don't appear in the empty position.
    std::map<std::string, std::set<std::string>> unusedPairMap;
    std::string firstChar, secondChar, key0, key1;
    for (const auto& pair : unusedPairs) {
        firstChar = pair.substr(0, 1);
        secondChar = pair.substr(1);
        key0 = firstChar + ".";
        key1 = "." + secondChar;
        if (unusedPairMap.count(key0) > 0) {
            unusedPairMap[key0].insert(secondChar);
        } else {
            unusedPairMap[key0] = {secondChar};
        }

        if (unusedPairMap.count(key1) > 0) {
            unusedPairMap[key1].insert(firstChar);
        } else {
            unusedPairMap[key1] = {firstChar};
        }
    }

    // Now construct the regexes.
    std::map<std::string, std::string> regexMap;
    for (auto const& [key, unusedLetters] : unusedPairMap) {
        size_t n = unusedLetters.size();
        std::string pattern = "";

        if (n == 0) {
            // This pattern matches with everything
            pattern = ".";
        } else if (n <= 13) {
            pattern = "[^";
            for (auto const& letter : unusedLetters) {
                pattern += letter;
            }
            pattern += "]";
        } else {
            // Compute {used letters} = {all letters} - {unused letters}
            std::set<std::string> usedLetters;
            std::set_difference(ALL_LETTERS.begin(), ALL_LETTERS.end(), unusedLetters.begin(), unusedLetters.end(),
                                std::inserter(usedLetters, usedLetters.end()));

            pattern = "[";
            for (auto const& letter : usedLetters) {
                pattern += letter;
            }
            pattern += "]";
        }
        regexMap[key] = pattern;
    }
    return regexMap;
}

/*
 * Generate regex for letter pairs.
 */
void DatasetManager::analyzeLetterPairs() const {

    std::vector<std::string> unusedStartingPairs = getUnusedLetterPairs("beginning");
    std::vector<std::string> unusedEndingPairs = getUnusedLetterPairs("end");

    data::startingPairRegex = unusedPairsToRegexMap(unusedStartingPairs);
    data::endingPairRegex = unusedPairsToRegexMap(unusedEndingPairs);
    data::startingPairRegex["."] = ".";
    data::endingPairRegex["."] = ".";
}