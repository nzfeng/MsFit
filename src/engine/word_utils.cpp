#include "msfit/engine/word_utils.h"

/*
 * Convert a GridWord into a std::string.
 */
std::string GridWordToString(GridWord& word) {

    size_t nChars = word.squares->size();
    for (size_t i = 0; i < nChars; i++) {
        word.squares[i]->getData();
    }
}