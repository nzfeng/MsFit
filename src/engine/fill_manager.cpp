#include "msfit/engine/fill_manager.h"

FillManager::FillManager(DatasetManager& datasetManager_) : datasetManager(datasetManager_) {}

/*
 * Filling a single word: The only constraints are the ones imposed by letters already in the given word.
 * Return the specified # of options.
 */
std::vector<std::string> FillManager::getWordFills(GridWord* word, std::string& message, int nOptions) {


    std::vector<std::string> matches;

    // If no word selected
    if (word == NULL) {
        message = "No word selected.";
        return matches;
    }

    // If no dataset
    if (datasetManager.words.size() == 0) {
        message = "No wordlist loaded.";
        std::cerr << message << std::endl;
        return matches;
    }

    // If word is already filled, return.
    if (!word->isOpen()) {
        message = "Word is already filled.";
        return matches;
    }


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

    if (nOptions != -1 && nOptions < matches.size()) matches.resize(nOptions);
    message = "Fills generated";
    return matches;
}

/*
 * Filling the remaining entire puzzle using depth-first search. Heuristic is to start from the longest unfilled word.
 * Set originally open squares to "autofill" pen mode, so autofilled entries show up in a different color.
 */