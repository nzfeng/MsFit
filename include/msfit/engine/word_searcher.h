#include "msfit/engine/dataset_manager.h"

class WordSearcher {
  public:
    WordSearcher(DatasetManager& datasetManager);
    ~WordSearcher();

  private:
    DatasetManager& datasetManager;
};