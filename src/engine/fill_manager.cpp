#include <map>
#include <random>

#include "msfit/engine/fill_manager.h"

FillManager::FillManager(DatasetManager& datasetManager_) : datasetManager(datasetManager_) {}

/*
 * Filling a single word: The only constraints are the ones imposed by letters already in the given word.
 * Return the specified # of options.
 */
std::vector<std::string> FillManager::getWordFills(GridWord* word, std::string& message, int nOptions) const {


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
 *      - Heuristic is to start from the word with the fewest fill options. Usually this is longest unfilled word to
 *      start; as the puzzle progresses, it will be the words that have started to get filled in.
 *      - At each step, attempt to fill a word by randomly selecting a fill option. No other optimizations.
 *      - After tentatively filling a word, check each of the words it intersects to see if they have >n fill options.
 *      If not, backtrack.
 *      - No other optimizations for terminating unpromising sub-trees early.
 *      - Just return the first grid fill found.
 *
 * Set originally open squares to "autofill" pen mode, so autofilled entries show up in a
 * different color.
 */
void FillManager::fillGridDFS(PuzzleGrid& puzzleGrid, std::string& message) const {

    // Convert the current puzzle into something with a more compact (smaller memory), and faster to do word-lookups on
    // (maybe?) Puzzle is defined as dense array of fillable squares (std::vector of strings) Each word is represented
    // as an array of ints indexing into the array of squares. Total set of words is fixed-length. For each word, keep
    // track of each word that intersects it (an int into the array of words.) Everything is index-based.
    std::vector<std::vector<Square>>& data = puzzleGrid.getData();
    std::vector<std::vector<GridWord>>& gridWords = puzzleGrid.getWords();
    std::map<Square*, size_t> squareToDenseIdx;
    std::vector<std::string> cells;
    std::vector<std::vector<size_t>> words;
    // the i-th entry contains the indices of all words that intersect the i-th word
    std::vector<std::vector<size_t>> xMap;
    // Fill cells.
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            if (!data[i][j].isSolid()) {
                squareToDenseIdx[&data[i][j]] = cells.size();
                cells.push_back(data[i][j].getData());
            }
        }
    }
    // Fill words & xMap.
    size_t nAcrossWords = gridWords[0].size();
    std::array<size_t, 2> wordIdxOffsets = {0, nAcrossWords};
    for (size_t i = 0; i < 2; i++) {
        size_t nWords = gridWords[i].size();
        for (size_t j = 0; j < nWords; j++) {
            std::vector<size_t> word;
            std::vector<size_t> crosses;
            for (size_t k = 0; k < gridWords[i][j].squares.size(); k++) {
                Square* sq = gridWords[i][j].squares[k];
                crosses.push_back(wordIdxOffsets[(i + 1) % 2] + sq->getWord((i + 1) % 2)); // assuming only across/down
                word.push_back(squareToDenseIdx[sq]);
            }
            words.push_back(word);
            xMap.push_back(crosses);
        }
    }

    // message = "No grid fills found.";
}