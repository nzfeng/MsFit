inline Glib::ustring GridWord::toUstring() {
    Glib::ustring str;
    size_t nChars = this->squares.size();
    for (size_t i = 0; i < nChars; i++) {
        str += this->squares[i]->getData();
    }
    return str;
}

inline bool GridWord::isOpen() {
    size_t nChars = this->squares.size();
    for (size_t i = 0; i < nChars; i++) {
        if (this->squares[i]->isEmpty()) return true;
    }
    return false;
}

inline size_t GridWord::length() { return this->squares.size(); }

inline std::regex GridWord::toRegex(bool ignorePenciled) const {
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
    return std::regex(pattern);
}