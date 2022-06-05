#include "msfit/engine/dataset_manager.h"

void DatasetManager::loadFromFile(std::string& message) {
    // Get all txt files in the data directory
    std::string path, num, fn;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        path = entry.path();

        // Determine the length of words in this file.
        fn = path.substr(0, path.find_last_of(".")); // strip the ".txt"
        num = fn.substr(fn.find_last_of("_") + 1);
        std::stringstream sstream(num);
        size_t n;
        sstream >> n;

        std::vector<std::string> words_n;
        std::ifstream file(path.c_str());
        std::string line;
        if (file.is_open()) {
            while (!file.eof()) {
                getline(file, line);
                if (line == "") continue;
                words_n.push_back(line);
            }
            file.close();
            words.emplace(n, words_n);
        } else {
            std::cerr << "Could not open file <" << path << ">!" << std::endl;
        }
    }
    if (words.size() > 0) {
        message = "Data loaded from file.";
        std::cout << message << std::endl;
    }
}

/*
 * Create a binary file from the txt wordlist.
 */
void DatasetManager::createBinary(std::string& message) {}


/*
 * Read from a saved binary file containing the entire wordlist.
 */
void DatasetManager::loadBinary(std::string& message) {}

void DatasetManager::loadData(std::string& message) {
    if (words.size() > 0) {
        message = "Data already loaded.";
        std::cout << message << std::endl;
        return;
    }

    std::ifstream file(binFile.c_str());
    if (!file.is_open()) {
        loadFromFile(message);
    }
    // TODO: else load from binary
}