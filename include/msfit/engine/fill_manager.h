#pragma once

#include <string_view>

#include "msfit/engine/dataset_manager.h"
#include "msfit/puzzle/grid_word.h"
#include "msfit/puzzle/puzzle_grid.h"

class FillManager {
  public:
    FillManager(DatasetManager& datasetManager);
    ~FillManager() {}

    std::vector<std::string> getWordFills(GridWord* word, std::string& message, bool ignorePenciled,
                                          bool gridFeasible = false, int nOptions = 10) const;
    void fillGridDFS(PuzzleGrid& puzzleGrid, std::string& message) const;

  private:
    DatasetManager& datasetManager;
    PuzzleGrid& puzzleGrid;

    // TODO: Function to determine unused letter pairs, instead of having it be static.

    // Map from a pair of chars to a string of letters that *never* appear in the blank spot, at the start of a word.
    std::map<std::string, std::string> unusedStartingLetterPairs = {{"A.", "J"},
                                                                    {".J", "WCMUGSLPDBVZITEKAYXRNOQJH"},
                                                                    {"B.", "GXVNWSZCFPMQTDJHK"},
                                                                    {".C", "YBRWZCFPMQDJUHK"},
                                                                    {".D", "GYXBRNVWLZPQMTDJ"},
                                                                    {".F", "WCFMGSLPDBVZTKYXRQJH"},
                                                                    {".G", "WCFMGSLPDBVZTKYXRNQJH"},
                                                                    {".H", "YXBVNLZIFQMEDJHK"},
                                                                    {".K", "WCFMGLPDBVZTEKYXRNQJH"},
                                                                    {".M", "YBRNVWLZFQMDJHK"},
                                                                    {".N", "YXBRNVLZCFQJH"},
                                                                    {".P", "YXBRVWZCFQPTDH"},
                                                                    {".Q", "WCFMUGLPDBVZTKYXRNOQJH"},
                                                                    {".S", "GYXBWSZCFQMDJHK"},
                                                                    {".T", "GYXBRNVWLZCFQMTDJK"},
                                                                    {".V", "WCFMGSLPDBVZKYXRNQJ"},
                                                                    {".W", "WCFMUGLPBVZIKYXRNJH"},
                                                                    {".X", "WCFMUGSLPDBVZITKYXRNQJH"},
                                                                    {".Z", "WCFMUGSLPDBVZITEKYXRNOQJH"},
                                                                    {"C.", "GXBVNWSZCFQPTJK"},
                                                                    {".B", "GYRVWSLZCQPDJHK"},
                                                                    {"D.", "GXBVSLZCFPMQTDJHK"},
                                                                    {".L", "YRNWZQMDJH"},
                                                                    {"E.", "OZJHK"},
                                                                    {".O", "XUEQ"},
                                                                    {"F.", "GYXVNWSZCFQPTMHK"},
                                                                    {".Y", "YVWIFQJUK"},
                                                                    {"G.", "GXBVWSZFQTDJK"},
                                                                    {"H.", "GXBRNWSLZCFQPMJHK"},
                                                                    {".R", "YRVNSLZQJH"},
                                                                    {"I.", "AYXWZIEJUH"},
                                                                    {".A", "XIUQ"},
                                                                    {".E", "XIUQ"},
                                                                    {".I", "IUQ"},
                                                                    {".U", "XIUZ"},
                                                                    {"J.", "WCFMGSLDBVZTKYXRNQJH"},
                                                                    {"K.", "GYXBVWSZCFQTMJHK"},
                                                                    {"L.", "GXBRNVWZFQMTDJHK"},
                                                                    {"M.", "GXVWSLZCFQMTDJHK"},
                                                                    {"N.", "GXRVNWLZQMTDJHK"},
                                                                    {"O.", "JQZ"},
                                                                    {"P.", "GXBVWZCFQPDJK"},
                                                                    {"Q.", "CFMGSLPDBVZITEKAYXRNOQJH"},
                                                                    {"R.", "GXBRNVWLZCFPMQTDJK"},
                                                                    {"S.", "GXBRVSZFJ"},
                                                                    {"T.", "GXZFQPTDJK"},
                                                                    {"U.", "AYXWOZCIQJUE"},
                                                                    {"V.", "GYXBRNVWZFPMQTDJHK"},
                                                                    {"W.", "GYXBVWSLZCFPMQTDJK"},
                                                                    {"X.", "WFUGSPDVZTEKAXNOQJH"},
                                                                    {"Y.", "WCFMGSLPDBVZTKYXRNQJH"},
                                                                    {"Z.", "WCFMUGSLPDBVZTKXRNQJH"}};
    // Map from a pair of chars to a string of letters that *never* appear in the blank spot, at the end of a word.
    std::map<std::string, std::string> unusedEndingLetterPairs = {{"A.", "JQ"},
                                                                  {".J", "BSXUWYRAZPGJOVDEMQNKTFHLIC"},
                                                                  {".Q", "BSXUWYRAZPGJOVDEMQNKTFHLIC"},
                                                                  {"B.", "GDVPXNUWFJHMZQKL"},
                                                                  {".D", "BGVPXYTFJMZQK"},
                                                                  {".F", "BDGVXNWYTJHMZQKC"},
                                                                  {".G", "BDVPSXYTFJHMZQKLC"},
                                                                  {".H", "BDVEXUWYRFJHMZQNLI"},
                                                                  {".K", "BDGPVXUTFJHMZQKI"},
                                                                  {".L", "BDGPSVXNTFJHZQKC"},
                                                                  {".M", "BDVPXNWTFJMZQKC"},
                                                                  {".N", "BDVSXYTFJZQKLC"},
                                                                  {".P", "BDGPXNWTFJHZQK"},
                                                                  {".U", "BDGXWYJZQPLI"},
                                                                  {".V", "BSXWYZPGJOVDMQNKFHLIC"},
                                                                  {".W", "BSXUWYRZPGJVDMQNKTHLIC"},
                                                                  {".X", "BDGPSVXWTRFJHZQKLC"},
                                                                  {".Z", "BSXUWYRPGJOVDMQKFHLC"},
                                                                  {"C.", "BGVXWFJMZQNLC"},
                                                                  {".B", "GVPSXNWYTFJHZQKC"},
                                                                  {".C", "GVPXUWYFJHMZQKC"},
                                                                  {"D.", "GVPXNUWTFJHMZQKL"},
                                                                  {".T", "GDVYJMZQK"},
                                                                  {"E.", "JQHI"},
                                                                  {".I", "VEYZQ"},
                                                                  {"F.", "BDGPVXNRJHMZQKLC"},
                                                                  {".R", "GVPSXNWTFJHZQKL"},
                                                                  {"G.", "BDVPXUWTRFJZQKLC"},
                                                                  {"H.", "BGVPXWRFJHZQKLC"},
                                                                  {"I.", "VUWJHQK"},
                                                                  {"J.", "BSXUWYRAZPGJVDEMQNKTFHLC"},
                                                                  {".A", "JXQW"},
                                                                  {".E", "JQ"},
                                                                  {".S", "JX"},
                                                                  {".Y", "JQY"},
                                                                  {"K.", "BDGPVXNWTRFJMZQKLC"},
                                                                  {"L.", "GVXUWRJHZQN"},
                                                                  {"M.", "GDVWTFJHMZQKC"},
                                                                  {"N.", "BVWRFJHMQPL"},
                                                                  {"O.", "JVQZ"},
                                                                  {"P.", "BDGPVXUWRJMZQKLC"},
                                                                  {"Q.", "BXUWYRAZPGJOVDEMQNKTFHLIC"},
                                                                  {".O", "XQ"},
                                                                  {"R.", "XWJHZQ"},
                                                                  {"S.", "BGVXWRJZQNL"},
                                                                  {"T.", "BDGPXNWRFJMQKL"},
                                                                  {"U.", "WJHZQKC"},
                                                                  {"V.", "BXWRZGJVDMQNKTFHLIC"},
                                                                  {"W.", "BVXUWRFAJHMZQPC"},
                                                                  {"X.", "BSXUWRAZPGJOVDMQNKFHLC"},
                                                                  {"Y.", "BDGVUWYTFJHZQNIC"},
                                                                  {"Z.", "BXUWRPGJVDMQNKTFHLIC"}};
    std::map<std::string, std::string> unusedMiddleLetterPairs;
};
