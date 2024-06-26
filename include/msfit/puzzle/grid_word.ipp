inline Glib::ustring GridWord::toUstring() const {
    Glib::ustring str;
    size_t nChars = this->squares.size();
    for (size_t i = 0; i < nChars; i++) {
        str += this->squares[i]->getData();
    }
    return str;
}

inline bool GridWord::isOpen() const {
    size_t nChars = this->squares.size();
    for (size_t i = 0; i < nChars; i++) {
        if (this->squares[i]->isEmpty()) return true;
    }
    return false;
}

inline size_t GridWord::length() const { return this->squares.size(); }

inline std::string GridWord::toRegexPattern(bool ignorePenciled) const {
    std::string pattern = "";
    size_t nChars = this->squares.size();
    for (size_t i = 0; i < nChars; i++) {
        if (this->squares[i]->getUtensil() == cell::PENCIL && ignorePenciled) {
            pattern += ".";
        } else if (this->squares[i]->isEmpty()) {
            pattern += ".";
        } else {
            pattern += this->squares[i]->getData();
        }
    }
    return pattern;
}

inline std::regex GridWord::toRegex(bool ignorePenciled) const {
    return std::regex(this->toRegexPattern(ignorePenciled));
}