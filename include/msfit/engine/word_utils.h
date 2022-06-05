#pragma once

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "msfit/puzzle/grid_word.h"

// A bunch of functions quantifying the "quality" of the word:
// 		- Quantify the "entropy" of the word (how likely it is to allow other cross-words)