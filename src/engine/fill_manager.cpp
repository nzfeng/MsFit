#include <map>
#include <random>

#include "msfit/engine/fill_manager.h"

FillManager::FillManager(DatasetManager& datasetManager_, PuzzleGrid& puzzleGrid_)
    : datasetManager(datasetManager_), puzzleGrid(puzzleGrid_) {}

/*
 * Given a word fill pattern, return all possible options.
 */
std::vector<std::string> FillManager::getAllWordFills(const std::string& regexPattern) const {

    size_t n = regexPattern.length();

    HashMap& hashMap = datasetManager.hashMaps[n];
    // TODO: Doing all these set intersections will be really slow. Look into google::dense_hash_set, etc.
    std::set<size_t> allMatches;
    for (size_t i = 0; i < n; i++) {
        if (regexPattern[i] != '.') {
            std::string pattern(n, '.');
            pattern[i] = regexPattern[i];
            std::set<size_t>& matches = hashMap[pattern];
            if (allMatches.size() != 0) {
                std::set<size_t> temp;
                std::set_intersection(allMatches.begin(), allMatches.end(), matches.begin(), matches.end(),
                                      std::inserter(temp, temp.end()));
                allMatches = temp;
            } else {
                allMatches = matches;
            }
        }
    }

    // Convert indices to strings.
    const std::vector<std::string>& options = datasetManager.words[n];
    std::vector<std::string> matches;
    for (auto& idx : allMatches) {
        matches.push_back(options[idx]);
    }
    return matches;
}

/*
 * Filling a single word: The only constraints are the ones imposed by letters already in the given word.
 * In other words, we only consider the current word in isolation; we do not consider how crossing words might narrow
 * the space of possible fills.
 *
 * If you *do* want to limit fills to only ones that allow possibilites for all intersecting words, set <gridFeasible>
 * to true.
 *
 * Return the specified # of options.
 *
 * TODO: Only add message to dialog if explicitly specified.
 */
std::vector<std::string> FillManager::getWordFills(GridWord* word, bool ignorePenciled, const std::string& constraint,
                                                   bool printMessage, int nOptions) const {


    std::vector<std::string> matches;

    // If no word selected
    if (word == NULL) {
        if (printMessage) bottomMenuContainer->addMessageToList("No word selected.");
        return matches;
    }

    // If no dataset
    if (datasetManager.words.size() == 0) {
        if (printMessage) bottomMenuContainer->addMessageToList("No wordlist loaded.");
        return matches;
    }

    // If word is already filled, return.
    if (!word->isOpen()) {
        if (printMessage) bottomMenuContainer->addMessageToList("Word is already filled.");
        return matches;
    }

    std::smatch match;
    std::regex pattern;
    if (constraint == "Grid-feasible") {
        pattern = getGridFeasibleRegex(word, ignorePenciled);
    } else {
        pattern = word->toRegex(ignorePenciled);
    }

    // Get all options.
    // auto t1 = high_resolution_clock::now();
    matches = getAllWordFills(word->toRegexPattern(ignorePenciled));
    // auto t2 = high_resolution_clock::now();
    // auto ns_int = duration_cast<nanoseconds>(t2 - t1);
    // std::cerr << "Regex time: " << ns_int.count() / 1e9 << "s" << std::endl;

    if (constraint == "Grid-compliant") {
        matches = getGridCompliantWords(word, matches, ignorePenciled);
    }

    // Return the specified number of matches.
    size_t nMatches = matches.size();
    std::string message = "Fills generated: " + std::to_string(nMatches) + " matches.";
    if (printMessage) bottomMenuContainer->addMessageToList(message);
    if (nOptions != -1 && (size_t)nOptions < nMatches) matches.resize(nOptions);
    return matches;
}

/*
 * Return true if the given word, and its current fill pattern, allow possible fills.
 * This is purely based on the current fill pattern, not any relationship with other words.
 */
bool FillManager::doFillsExist(GridWord* word, bool ignorePenciled) const {

    std::smatch match;
    const std::regex pattern = word->toRegex(ignorePenciled);
    size_t n = word->length();
    std::vector<std::string>& options = datasetManager.words[n];
    for (const auto& option : options) {
        if (std::regex_match(option, match, pattern)) {
            return true;
        }
    }
    return false;
}

/*
 * Given a set of matches, narrow them down to the ones that are grid-compliant.
 */
std::vector<std::string> FillManager::getGridCompliantWords(GridWord* word, const std::vector<std::string>& matches,
                                                            bool ignorePenciled) const {

    // For each match, go through each square and make sure it results at least 1 fill option for the crossing word.
    std::vector<std::string> winnowed;
    int crossType = word->isAcross; // assumes ACROSS = 0, DOWN = 1
    size_t n = word->length();
    Square* sq;
    for (const auto& match : matches) {
        bool compliant = true;
        for (size_t i = 0; i < n; i++) {
            sq = word->squares[i];
            if (!sq->isEmpty()) continue;

            sq->setData(match.substr(i, 1));

            // Which word intersects the current word at this square.
            GridWord& x = puzzleGrid.getWordFromSquare(sq, crossType);
            if (!doFillsExist(&x, ignorePenciled)) {
                compliant = false;
                sq->clearData();
                break;
            }
            sq->clearData();
        }
        if (!compliant) continue;
        winnowed.push_back(match);
    }
    return winnowed;
}

/*
 * Construct a regex pattern to find grid-feasible fills for the given word.
 *
 * Generating strictly "grid-compliant" fills would mean also evaluating the regex for each intersecting word, and only
 * returning fills that result in more than zero fills for every intersecting word. However, this function instead
 * returns all fills that merely "grid-plausible": all fills that result in legal letter-pairs that appear in the
 * English language.
 *
 * The reasoning is that (1) the set of grid-compliant fills is probably too strict, especially when the wordset is
 * incomplete (which it always will be); and (2) the set of grid-plausible fills is probably not that much larger than
 * the set of grid-compliant fills anyway. The goal of autofill tools is to alleviate the computational burden of
 * finding a crossword-compliant combination of words, and help guide the constructor towards a legal crossword; along
 * the way, the constructor will likely think of good entries to complete partially-filled words that may not appear in
 * wordlists.
 */
std::regex FillManager::getGridFeasibleRegex(GridWord* word, bool ignorePenciled) const {

    // Build the regex pattern.
    std::string pattern = "";
    int crossType = word->isAcross; // assumes ACROSS = 0, DOWN = 1
    for (auto sq : word->squares) {
        if (!sq->isEmpty()) {
            pattern += sq->getData();
            continue;
        }

        // Which word intersects the current word at this square.
        GridWord& x = puzzleGrid.getWordFromSquare(sq, crossType);
        size_t n = x.length();
        // Index of <sq> in this word.
        size_t charIndex = sq->getIndexOfCharInWord(crossType);

        // Determine if this square is the first/second, or last/second-to-last in the word.
        // I wish I could come up with a more elegant way to do this...
        std::string nextChar;
        std::string pair;
        Square* nextSq;
        if (charIndex == 0) {
            nextSq = x.squares[charIndex + 1];
            nextChar = (nextSq->getUtensil() == cell::PENCIL) && ignorePenciled ? "" : nextSq->getData();
            pair = "." + nextChar;
            pattern += data::startingPairRegex[pair];
        } else if (charIndex == n - 2) {
            nextSq = x.squares[charIndex + 1];
            nextChar = (nextSq->getUtensil() == cell::PENCIL) && ignorePenciled ? "" : nextSq->getData();
            pair = "." + nextChar;
            pattern += data::endingPairRegex[pair];
        } else if (charIndex == 1) {
            nextSq = x.squares[charIndex - 1];
            nextChar = (nextSq->getUtensil() == cell::PENCIL) && ignorePenciled ? "" : nextSq->getData();
            pair = nextChar + ".";
            pattern += data::startingPairRegex[pair];
        } else if (charIndex == n - 1) {
            nextSq = x.squares[charIndex - 1];
            nextChar = (nextSq->getUtensil() == cell::PENCIL) && ignorePenciled ? "" : nextSq->getData();
            pair = nextChar + ".";
            pattern += data::endingPairRegex[pair];
        } else {
            pattern += ".";
        }
    }
    return std::regex(pattern);
}

/*
 * Determine the current word in the grid with the fewest possible fills (i.e., the most "constrained" word.)
 *
 * <constraint> = "Grid-feasible", "Grid-compliant", "None"
 */
GridWord* FillManager::getMostConstrainedWord(bool ignorePenciled, const std::string& constraint) {

    std::vector<std::vector<GridWord>>& gridWords = puzzleGrid.getWords();

    size_t minFills = 100000;
    GridWord* minWord = NULL;
    for (size_t i = 0; i < gridWords.size(); i++) {
        for (size_t j = 0; j < gridWords[i].size(); j++) {
            if (!gridWords[i][j].isOpen()) continue;
            size_t nFills = getWordFills(&gridWords[i][j], ignorePenciled, constraint).size();
            if (nFills < minFills) {
                minWord = &gridWords[i][j];
                minFills = nFills;
            }
        }
    }
    bottomMenuContainer->addMessageToList("Most constrained word has " + std::to_string(minFills) + " fills.");
    return minWord;
}

/*
 * Helper function for fillGridDFS().
 *
 * <cells> is a dense array of all the write-able cells in the puzzle.
 * <words> is a dense array representing all the words in the puzzle. Each item is a sequence of indices into <cells>.
 * <xMap> is a dense array where the i-th entry contains the indices of all words that intersect the i-th word.
 * <cellToSquare> is a map from <cells> to the original Squares* in the puzzle.
 */
void FillManager::buildSearchStructures(std::vector<std::string>& cells, std::vector<std::vector<size_t>>& words,
                                        std::vector<std::vector<size_t>>& xMap,
                                        std::vector<Square*>& cellToSquare) const {

    cells.clear();
    words.clear();
    xMap.clear();
    cellToSquare.clear();

    std::vector<std::vector<Square>>& data = puzzleGrid.getData();
    std::vector<std::vector<GridWord>>& gridWords = puzzleGrid.getWords();
    std::map<Square*, size_t> squareToDenseIdx;

    // Fill cells.
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            if (!data[i][j].isSolid()) {
                size_t idx = cells.size();
                squareToDenseIdx[&data[i][j]] = idx;
                cells.push_back(data[i][j].getData());
                cellToSquare.push_back(&data[i][j]);
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
}


/*
 * Filling the remaining entire puzzle using depth-first search:
 *      - Heuristic is to start from the word with the fewest fill options. Usually this is longest unfilled word to
 *      start; as the puzzle progresses, it will be the words that have started to get filled in. The idea is to prune
 *      unpromising sub-trees ASAP.
 *      - At each step, attempt to fill a word by randomly selecting a fill option. Currently, no other optimizations.
 *      - After tentatively filling a word, check each of the words it intersects to see if they have >n fill options.
 *      If not, backtrack.
 *      - Currently, no other optimizations for terminating unpromising sub-trees early.
 *      - Just return the first grid fill found.
 *
 * Warning: The only solutions returned by this method are ones that only contain words from the wordlist.
 *
 * Set originally open squares to "autofill" pen mode, so autofilled entries show up in a
 * different color.
 */
void FillManager::fillGridDFS() const {

    // Convert the current puzzle into something with a more compact (smaller memory), and faster to do word-lookups on.
    // Everything is index-based; hopefully the time savings add up vs. pointer-chasing (which is how the data in
    // PuzzleGrid is implemented.)

    // <cells> is a dense array of all the write-able cells in the puzzle.
    // <words> is a dense array representing all the words. Each item (word) is a sequence of indices into <cells>.
    // <xMap> is a dense array where the i-th entry contains the indices of all words that intersect the i-th word.
    // <cellToSquare> is a map from <cells> to the original Squares* in the puzzle.
    std::vector<std::string> cells;
    std::vector<std::vector<size_t>> words;
    std::vector<size_t> nFills; // map from word index to number of potential fills
    std::vector<std::vector<size_t>> xMap;
    std::vector<Square*> cellToSquare;
    buildSearchStructures(cells, words, xMap, cellToSquare);

    // Initialize the number of fills for every word.
    nFills.resize(words.size());
    for (size_t i = 0; i < words.size(); i++) {
        nFills[i] = datasetManager.words[words[i].size()].size();
    }

    // At each step, start at the incomplete word with the fewest fills.
    // Need to keep track of which entries were tried (which sub-trees were pruned.) For each entry, keep an array that
    // indexes into the whole list of available words of that length; delete entries as they get used. Don't prune if
    // the word that has zero fills is a word that was manually filled to begin with.
    // Also need to make sure no duplicate entries in the puzzle at each step.

    // TODO: Write a recursive helper function with all state variables passed in as arguments. Try a fill. If
    // termination criteria is met, return.

    // Fill in empty cells if a solution was found.
    for (size_t i = 0; i < cells.size(); i++) {
        Square* sq = cellToSquare[i];
        if (sq->isEmpty()) {
            sq->setUtensil(cell::AUTOFILL);
            sq->setData(cells[i]);
        }
    }
    puzzleGrid.queue_draw();

    // bottomMenuContainer->addMessageToList("No grid fills found.");
}

// TODO: a function that returns partial solutions; somehow indicates which areas of the grid are difficult to fill