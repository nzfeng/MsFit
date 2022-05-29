#pragma once

#include "msfit/puzzle/square.h"

struct GridWord {
    bool isAcross;
    std::vector<Square*> squares;
};