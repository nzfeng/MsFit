#include "msfit/engine/fill_manager.h"

FillManager::FillManager(DatasetManager& datasetManager_) : datasetManager(datasetManager_) {}

/*
 * Filling a single word: The only constraints are the ones imposed by letters already in the given word.
 * Return the specified # of options.
 */
std::vector<std::string> FillManager::getWordFills(GridWord* word, int nOptions) {


    std::vector<std::string> matches;

    // If no word selected
    if (word == NULL) return matches;

    // If no dataset
    if (datasetManager.words.size() == 0) {
        std::cerr << "No wordlist loaded" << std::endl;
        return matches;
    }

    // If word is already filled, return.
    if (!word->isOpen()) return matches;

    // Look through all options for now
    size_t n = word->length();
    std::vector<std::string>& options = datasetManager.words[n];
    std::smatch match;
    const std::regex pattern = word->toRegex();
    for (const auto& option : options) {
        if (std::regex_match(option, match, pattern)) {
            matches.push_back(match.str());
        }
    }
    // TODO: maybe don't go through all possible options
    if (nOptions < matches.size()) matches.resize(nOptions);
    return matches;
}

/*
 * Filling the remaining entire puzzle using depth-first search. Heuristic is to start from the longest unfilled word.
 */