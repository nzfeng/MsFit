inline Glib::ustring Square::getData() const { return data; }

inline void Square::setData(const Glib::ustring& str) { data = str; }

inline void Square::setSize(int width_) { width = width_; }

inline int Square::getSize() const { return width; }

inline bool Square::isSolid() const { return solid; }

inline void Square::setNumber(int num) { number = num; }
inline int Square::getNumber() const { return number; }