#include "worker.hpp"

#include <iostream>

inline std::filesystem::path Worker::new_path(const std::filesystem::path & path, const std::string & name) {
    return  path.parent_path() / name;
}

bool Worker::sync_photos(work_type type, bool verbose = false, bool ask = false) {
    return false;
}

bool Worker::rename_photos(const std::string & name, work_type type, bool verbose = false, bool ask = false) {
    return false;
}

void Worker::rename_folder(const std::string & name, work_type type) {
    switch (type) {
        case original:
            std::filesystem::rename(src, Worker::new_path(src, name));
            break;
        case both:
            std::filesystem::rename(src, Worker::new_path(src, name));
        case converted:
            std::filesystem::rename(dest, Worker::new_path(dest, name));
            break;
    }
}

void Worker::read_raw_files(std::vector<std::string> & files) {
    Worker::read_files(files, raw_suffix, src);
}

void Worker::read_converted_files(std::vector<std::string> & files) {
    Worker::read_files(files, converted_suffix, dest);
}

void Worker::read_files(std::vector<std::string> & files, const std::string & suffix, const std::filesystem::path & path) {
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        if (entry.path().extension() == suffix)
            files.push_back(entry.path().filename());
    }
}