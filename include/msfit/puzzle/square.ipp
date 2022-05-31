inline Glib::ustring Square::getData() const { return data; }

inline void Square::setData(const Glib::ustring& str) { data = str; }

inline void Square::setSize(int width_) { width = width_; }

inline int Square::getSize() const { return width; }

inline bool Square::isSolid() const { return solid; }
inline void Square::toggleSolid() { solid = !solid; }

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

// inline void Square::setDownWord(GridWord* word_) { downWord = word_; }
// inline GridWord* Square::getDownWord() const { return downWord; }
// inline void Square::setAcrossWord(GridWord* word_) { acrossWord = word_; }
// inline GridWord* Square::getAcrossWord() const { return acrossWord; }

inline void Square::setDownWord(size_t index) { downWord = index; }
inline size_t Square::getDownWord() const { return downWord; }
inline void Square::setAcrossWord(size_t index) { acrossWord = index; }
inline size_t Square::getAcrossWord() const { return acrossWord; }

inline void Square::setDownWordIndex(size_t index) { downWordIndex = index; }
inline void Square::setAcrossWordIndex(size_t index) { acrossWordIndex = index; }
inline size_t Square::getAcrossWordIndex() const { return acrossWordIndex; }
inline size_t Square::getDownWordIndex() const { return downWordIndex; }

inline void Square::setSelectionStatus(int status) { selectionStatus = status; }