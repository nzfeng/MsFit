#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "msfit/puzzle/grid_word.h"

// Translate a GridWord into a std::string


// A bunch of functions quantifying the "quality" of the word:
// 		- Quantify the "entropy" of the word (how likely it is to allow other cross-words)