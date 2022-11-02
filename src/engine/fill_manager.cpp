#include "msfit/engine/fill_manager.h"

FillManager::FillManager(DatasetManager& datasetManager_, PuzzleGrid& puzzleGrid_)
    : datasetManager(datasetManager_), puzzleGrid(puzzleGrid_) {}

/*
 * Helper function to perform set intersection in-place (replacing setA.)
 * https://stackoverflow.com/a/1773620
 */
template <typename T>
void FillManager::setIntersectionInPlace(std::set<T>& setA, const std::set<T>& setB) const {

    typename std::set<T>::iterator itA = setA.begin();
    typename std::set<T>::iterator itB = setB.begin();
    while ((itA != setA.end()) && (itB != setB.end())) {
        if (*itA < *itB) {
            setA.erase(itA++);
        } else if (*itB < *itA) {
            ++itB;
        } else {
            ++itA;
            ++itB;
        }
    }
    setA.erase(itA, setA.end());
}

/*
 * Helper function for getAllWordFills(). Returns indices into datasetManager.words[n], but doesn't map them to words.
 */
std::set<size_t> FillManager::getAllWordFillIndices(const std::string& regexPattern) const {

    size_t n = regexPattern.length();

    // empty pattern just yields the whole list
    std::string emptyPattern(n, '.');
    if (regexPattern == emptyPattern) {
        std::set<size_t> indices;
        for (size_t i = 0; i < datasetManager.words[n].size(); i++) indices.insert(i);
        return indices;
    }

    HashMap& hashMap = datasetManager.hashMaps[n];

    // // TODO: For some reason, this is super slow when doing autofill (and only when doing autofill...)
    // if (n <= data::N_MAX_QUERY) {
    //     std::set<size_t>& matches = hashMap[regexPattern];
    //     return matches;
    // }

    // TODO: Doing all these set intersections will be really slow. Look into google::dense_hash_set, etc.
    bool initalized = false;
    std::set<size_t> allMatches;
    for (size_t i = 0; i < n; i++) {
        if (regexPattern[i] != '.') {
            std::string pattern(n, '.');
            pattern[i] = regexPattern[i];
            const std::set<size_t>& matches = hashMap[pattern];
            if (initalized) {
                setIntersectionInPlace(allMatches, matches);
            } else {
                allMatches = matches;
                initalized = true;
            }
        }
    }

    return allMatches;
}

/*
 * Given a word fill pattern, return all possible options.
 */
std::vector<std::string> FillManager::getAllWordFills(const std::string& regexPattern) const {

    size_t n = regexPattern.length();

    // empty pattern just yields the whole list
    std::string emptyPattern(n, '.');
    if (regexPattern == emptyPattern) return datasetManager.words[n];

    std::set<size_t> allMatches = getAllWordFillIndices(regexPattern);

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

    // std::smatch match;
    // const std::regex pattern = word->toRegex(ignorePenciled);
    // size_t n = word->length();
    // std::vector<std::string>& options = datasetManager.words[n];
    // for (const auto& option : options) {
    //     if (std::regex_match(option, match, pattern)) {
    //         return true;
    //     }
    // }
    // return false;

    // TODO: no need to compute the whole word list...
    std::set<size_t> fills = getAllWordFillIndices(word->toRegexPattern(ignorePenciled));
    return fills.size() > 0;
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
            // TODO: better to work directly with regex patterns rather than changing the grid itself
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

    auto t1 = high_resolution_clock::now();

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

    auto t2 = high_resolution_clock::now();
    auto ns_int = duration_cast<nanoseconds>(t2 - t1);
    std::cerr << "Get most constrained time: " << ns_int.count() / 1e9 << "s" << std::endl;

    bottomMenuContainer->addMessageToList("Most constrained word has " + std::to_string(minFills) + " fills.");
    return minWord;
}

/*
 * Helper function for fillGridDFS().
 *
 * <cells> is a dense array of all the write-able cells in the puzzle.
 * <words> is a dense array representing all the words in the puzzle. Each item is a sequence of indices into <cells>.
 * <xMap> is a dense array where the i-th entry contains the indices of all words that contain the i-th cell.
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
    std::map<Square*, size_t> squareToDenseIdx; // Square in puzzleGrid to index in <cells>

    // Fill cells.
    // TODO: If square data is in autofill mode, treat it as an empty square.
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            Square& sq = data[i][j];
            if (!sq.isSolid()) {
                size_t idx = cells.size();
                squareToDenseIdx[&sq] = idx;
                cells.push_back(sq.getData());
                cellToSquare.push_back(&sq);
            }
        }
    }
    // Fill words & xMap.
    // Warning: This code block assumes words are only across/down in a grid.
    xMap.resize(cells.size());
    size_t nAcrossWords = gridWords[0].size();
    std::array<size_t, 2> wordIdxOffsets = {0, nAcrossWords};
    for (size_t i = 0; i < 2; i++) {
        size_t nWords = gridWords[i].size();
        for (size_t j = 0; j < nWords; j++) {
            std::vector<size_t> word;
            for (size_t k = 0; k < gridWords[i][j].squares.size(); k++) {
                Square* sq = gridWords[i][j].squares[k];
                word.push_back(squareToDenseIdx[sq]);
                xMap[squareToDenseIdx[sq]].push_back(words.size());
            }
            words.push_back(word);
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
 *
 * <timeLimit> = time limit in seconds
 */
void FillManager::fillGridDFS(const double timeLimit) {

    // Convert the current puzzle into something with a more compact (smaller memory), and faster to do word-lookups on.
    // Everything is index-based; hopefully the time savings add up vs. pointer-chasing (which is how the data in
    // PuzzleGrid is implemented.)

    // <cells> is a dense array of all the write-able cells in the puzzle.
    // <words> is a dense array representing all the words. Each item (word) is a sequence of indices into <cells>.
    // <xMap> is a dense array where the i-th entry contains the indices of all words that contain the i-th cell.
    // <cellToSquare> is a map from <cells> to the original Squares* in the puzzle.
    std::vector<std::string> cells;
    std::vector<std::vector<size_t>> words;
    std::vector<std::vector<size_t>> xMap;
    std::vector<Square*> cellToSquare;
    buildSearchStructures(cells, words, xMap, cellToSquare);

    // TODO: At each step, start at the incomplete word with the fewest fills. This may be slower, though.
    auto t1 = high_resolution_clock::now();

    // Call a recursive helper function with all state variables passed in as arguments. Try a fill. If
    // termination criteria is met, return.
    std::vector<std::string> wordsUsedSoFar;
    std::vector<std::vector<GridWord>>& gridWords = puzzleGrid.getWords();
    for (const auto& lst : gridWords) {
        for (const auto& gridword : lst) {
            if (!gridword.isOpen()) wordsUsedSoFar.push_back(gridword.toUstring());
        }
    }
    fillGridDFSHelper(cells, words, xMap, wordsUsedSoFar);

    auto t2 = high_resolution_clock::now();
    auto ns_int = duration_cast<nanoseconds>(t2 - t1);
    std::cerr << "Autofill took " << ns_int.count() / 1e9 << "s" << std::endl;

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

/*
 * Recursive helper function for fillGridDFS().
 * <state>: same length as <words>. The i-th entry is the set of indices into datasetManager.words[n], representing
 * which words have already been tried for this entry, at this stage of the search.
 * <wordsUsedSoFar> = a list of all words used so far; used to avoid adding
 * duplicate words to the puzzle.
 *
 * Return false if choosing a particular fill results in a dead end. Return true otherwise (we've arrived our
 * destination, i.e. a completed puzzle.)
 */
bool FillManager::fillGridDFSHelper(std::vector<std::string>& cells, const std::vector<std::vector<size_t>>& words,
                                    const std::vector<std::vector<size_t>>& xMap,
                                    std::vector<std::string>& wordsUsedSoFar) {

    // Currently, getMostConstrainedWord() is rather slow; it will be faster to simply try to fill words as we go, and
    // terminate a path when we reach an entry with zero possible fills. Right now, just go in some random permutation
    // of <words>.
    size_t nWords = words.size();
    std::vector<size_t> wordOrder(nWords);
    std::iota(wordOrder.begin(), wordOrder.end(), 0);
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(wordOrder), std::end(wordOrder), rng);

    // TODO: Return the furthest solution obtained.
    for (const auto& i : wordOrder) {
        // Determine if word is incomplete.
        const std::vector<size_t>& word = words[i];
        size_t n = word.size();
        for (const auto& cellIdx : word) {
            if (cells[cellIdx] == "") {
                // Word is incomplete; determine possible fills. Need to make sure all filled-in words are valid.
                std::vector<std::string> fills = getGridCompliantFills(i, cells, words, xMap, wordsUsedSoFar);
                // Get rid of any words already used in the puzzle.
                for (const auto& existingWord : wordsUsedSoFar) {
                    if (existingWord.size() == n) {
                        // Each wordset should have unique elements, so only need to erase one element.
                        // Omitting v.end() in the below line causes segfaults for some reason, though.
                        fills.erase(std::remove(fills.begin(), fills.end(), existingWord), fills.end());
                    }
                }
                if (fills.size() == 0) {
                    return false;
                }

                // Update cells with each possible fill at a time; call recursive function.
                // TODO: Use DFS/BFS, which I imagine would rule out dead ends faster than going in random word order
                // (because always starting from a point where words already are); also won't need a recursive function.
                // Look into Dijkstra.
                std::shuffle(std::begin(fills), std::end(fills), rng);
                for (const auto& possibleFill : fills) {
                    std::map<size_t, std::string> origFill; // save original fill
                    for (size_t k = 0; k < n; k++) {
                        size_t idx = word[k];
                        origFill.insert({idx, cells[idx]});
                        cells[idx] = possibleFill[k];
                    }
                    wordsUsedSoFar.push_back(possibleFill);
                    if (!fillGridDFSHelper(cells, words, xMap, wordsUsedSoFar)) {
                        wordsUsedSoFar.pop_back();
                        // Restore previous fill.
                        for (const auto& pair : origFill) cells[pair.first] = pair.second;
                    } else {
                        return true;
                    }
                }
                return false; // if no fills worked for this entry, we've reached a dead end
                break; // don't need to consider further empty squares in the same word, we already determined its fills
            }
        }
    }

    // If we get through all words without detecting an incomplete word, that means we filled the grid. Return; the
    // final solution should be stored in <cells>.
    return true;
}

/*
 * A helper function specific to fillGridDFSHelper().
 */
std::vector<std::string> FillManager::getGridCompliantFills(const size_t& wordIdx,
                                                            const std::vector<std::string>& cells,
                                                            const std::vector<std::vector<size_t>>& words,
                                                            const std::vector<std::vector<size_t>>& xMap,
                                                            const std::vector<std::string>& wordsUsedSoFar) const {

    const std::vector<size_t>& word = words[wordIdx];
    std::string pattern = "";
    for (const auto& idx : word) pattern += (cells[idx] == "") ? "." : cells[idx];

    size_t n = word.size();
    std::vector<std::string> fills = getAllWordFills(pattern);
    // // Get rid of any words already used in the puzzle. Don't strictly need to do this, since duplicates are taken
    // // care of upon returning to fillGridDFSHelper().
    // for (const auto& existingWord : wordsUsedSoFar) {
    //     if (existingWord.size() == n) {
    //         fills.erase(std::remove(fills.begin(), fills.end(), existingWord), fills.end());
    //     }
    // }
    std::vector<std::string> compliantFills;

    // For each match, go through each square and make sure it results at least 1 fill option for the crossing word.
    for (const auto& fill : fills) {
        bool safe = true;
        for (size_t i = 0; i < n; i++) {
            size_t cellIdx = word[i];
            // Only check crossing words that are affected by the possible fill, since doing this kind of cross-checking
            // is relatively expensive.
            if (cells[cellIdx] != "") continue;

            for (const auto& xWordIdx : xMap[cellIdx]) {
                if (xWordIdx == wordIdx) continue;

                const std::vector<size_t>& xWord = words[xWordIdx];
                size_t xN = xWord.size();
                std::string xPattern = "";
                for (const auto& xIdx : xWord) {
                    if (xIdx == cellIdx) {
                        xPattern += fill[i];
                    } else {
                        xPattern += (cells[xIdx] == "") ? "." : cells[xIdx];
                    }
                }
                std::vector<std::string> xFills = getAllWordFills(xPattern);

                // Get rid of any words already used in the puzzle.
                for (const auto& existingWord : wordsUsedSoFar) {
                    if (existingWord.size() == xN) {
                        // Each wordset should have unique elements, so only need to erase one element.
                        // Omitting v.end() in the below line causes segfaults for some reason, though.
                        xFills.erase(std::remove(xFills.begin(), xFills.end(), existingWord), xFills.end());
                    }
                }

                if (xFills.size() == 0) {
                    safe = false;
                    break;
                }
            }
            if (!safe) break;
        }
        if (safe) compliantFills.push_back(fill);
    }

    return compliantFills;
}

// TODO: a function that returns partial solutions; somehow indicates which areas of the grid are difficult to fill