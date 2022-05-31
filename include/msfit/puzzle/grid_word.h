#pragma once

class Square; // forward declaration, since square.h also includes grid_word.h

struct GridWord {
    bool isAcross;
    std::vector<Square*> squares;
};