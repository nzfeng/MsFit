inline size_t PuzzleGrid::nRows() const { return data.size(); }
inline size_t PuzzleGrid::nCols() const { return data[0].size(); }

inline bool PuzzleGrid::isAcrossSelected() const { return acrossSelected; }