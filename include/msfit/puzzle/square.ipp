inline Glib::ustring Square::getData() const { return data; }

inline void Square::setData(const Glib::ustring& str) { data = str; }
inline void Square::clearData() { data = ""; }

inline int Square::getUtensil() const { return utensil; }
inline void Square::setUtensil(int utensil_) { utensil = utensil_; }

inline void Square::setSize(int width_) { width = width_; }

inline int Square::getSize() const { return width; }

inline bool Square::isSolid() const { return solid; }
inline void Square::setSolid(bool state) {
    clearData();
    solid = state;
}
inline void Square::toggleSolid() {
    clearData();
    solid = !solid;
}

inline void Square::setNumber(int num) { number = num; }
inline int Square::getNumber() const { return number; }

inline void Square::setLeft(Square* neighbor) { left = neighbor; }
inline void Square::setRight(Square* neighbor) { right = neighbor; }
inline void Square::setAbove(Square* neighbor) { above = neighbor; }
inline void Square::setBelow(Square* neighbor) { below = neighbor; }
inline Square* Square::getLeft() const { return left; }
inline Square* Square::getRight() const { return right; }
inline Square* Square::getAbove() const { return above; }
inline Square* Square::getBelow() const { return below; }

inline void Square::setPosition(int i, int j) { position = {i, j}; }
inline std::array<int, 2> Square::getPosition() const { return position; }

inline void Square::setWord(size_t index, int wordtype) { wordIndex[wordtype] = index; }
inline size_t Square::getWord(int wordtype) const { return wordIndex[wordtype]; }

inline void Square::setIndexOfCharInWord(size_t index, int wordtype) { charIndex[wordtype] = index; }
inline size_t Square::getIndexOfCharInWord(int wordtype) const { return charIndex[wordtype]; }

inline void Square::setSelectionStatus(int status) { selectionStatus = status; }

inline bool Square::isEmpty() const { return data == ""; }