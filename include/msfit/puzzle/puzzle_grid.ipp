inline size_t PuzzleGrid::nRows() const { return data.size(); }
inline size_t PuzzleGrid::nCols() const { return data[0].size(); }

inline size_t PuzzleGrid::nWords() const {
    return gridWords[grid::wordtype::ACROSS].size() + gridWords[grid::wordtype::DOWN].size();
}
inline size_t PuzzleGrid::nWhiteSquares() const {
    size_t n = 0;
    for (const auto& row : data) {
        for (const auto& sq : row) {
            if (!sq.isSolid()) n++;
        }
    }
    return n;
}
inline size_t PuzzleGrid::nBlackSquares() const {
    size_t n = 0;
    for (const auto& row : data) {
        for (const auto& sq : row) {
            if (sq.isSolid()) n++;
        }
    }
    return n;
}

inline std::vector<std::vector<Square>>& PuzzleGrid::getData() { return data; }
inline std::vector<std::vector<GridWord>>& PuzzleGrid::getWords() { return gridWords; }

inline bool PuzzleGrid::getWordMode() const { return wordtypeMode; }
inline void PuzzleGrid::setWordMode(int wordtype) { wordtypeMode = wordtype; }

inline void PuzzleGrid::setSelectedSquare(const std::array<int, 2>& indices) { selectedSquare = indices; }
inline std::array<int, 2> PuzzleGrid::getSelectedSquare() const { return selectedSquare; }