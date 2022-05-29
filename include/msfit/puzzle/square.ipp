inline Glib::ustring Square::getData() const { return data; }

inline void Square::setData(const Glib::ustring& str) { data = str; }

inline void Square::setSize(int width_) { width = width_; }

inline int Square::getSize() const { return width; }

inline bool Square::isSolid() const { return solid; }
inline bool Square::toggleSolid() { solid = !solid; }

inline void Square::setNumber(int num) { number = num; }
inline int Square::getNumber() const { return number; }

inline void Square::setDownWord(GridWord* word_) { downWord = word_; }
inline GridWord* Square::getDownWord() const { return downWord; }
inline void Square::setAcrossWord(GridWord* word_) { acrossWord = word_; }
inline GridWord* Square::getAcrossWord() const { return acrossWord; }

inline void Square::setSelectionStatus(int status) { selectionStatus = status; }