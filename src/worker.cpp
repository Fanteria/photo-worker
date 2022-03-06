#include "worker.hpp"

#include <iostream>

bool Worker::sync_photos(work_type type, bool verbose = false, bool ask = false) {
    return false;
}

bool Worker::rename_photos(const std::string & name, work_type type, bool verbose = false, bool ask = false) {
    return false;
}

bool Worker::rename_folder(const std::string & name, work_type type) {
    return false;
}

void Worker::read_raw_files(std::vector<std::string> & files) {
    for (const auto & entry : std::filesystem::directory_iterator(this->src)) {
        if (entry.path().extension() == suffix)
            files.push_back(entry.path().filename());
    }
}