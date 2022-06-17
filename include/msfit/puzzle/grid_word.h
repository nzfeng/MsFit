#pragma once

#include <regex>

#include "gtkmm/window.h" // just to get Glib::ustring

#include "msfit/puzzle/square.h"
// class Square; // forward declaration, since square.h also includes grid_word.h

struct GridWord {
    bool isAcross;
    std::vector<Square*> squares;

    // Convert a GridWord into a Glib::ustring.
    Glib::ustring toUstring(); // no chars for spaces
    // Translate a GridWord into a std::string?

    // To a regex expression ready to be used by std::regex.
    // This only considers the word in isolation; does not consider how crossing words may limit choices.
    std::regex toRegex(bool ignorePenciled) const;

    // Return true if the word has not been completely filled.
    bool isOpen();
    // Return the length of the word.
    size_t length();
};

#include "msfit/puzzle/grid_word.ipp"