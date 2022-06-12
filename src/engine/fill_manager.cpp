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
    size_t nMatches = matches.size();
    message = "Fills generated: " + std::to_string(nMatches) + " matches.";
    if (nOptions != -1 && (size_t)nOptions < nMatches) matches.resize(nOptions);
    return matches;
}

/*
 * Filling the remaining entire puzzle using depth-first search:
 *      - Heuristic is to start from the longest unfilled word.
 *      - At each step, attempt to fill a word by randomly selecting a fill option. No other optimizations.
 *      - After tentatively filling a word, check each of the words it intersects to see if they have >0 fill options.
 *      If not, backtrack.
 *      - No other optimizations for terminating unpromising sub-trees early.
 *
 * Set originally open squares to "autofill" pen mode, so autofilled entries show up in a
 * different color.
 */