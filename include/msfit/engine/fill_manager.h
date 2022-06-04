#include "msfit/engine/dataset_manager.h"

class FillManager {
  public:
    FillManager(DatasetManager& datasetManager);
    ~FillManager();

  private:
    DatasetManager& datasetManager;
};