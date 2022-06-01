#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// Translate GridWord into a form that can easily be regex-ed

// A bunch of functions quantifying the "quality" of the word:
// 		- Quantify the "entropy" of the word (how likely it is to allow other cross-words)