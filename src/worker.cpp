#include "worker.hpp"

#include <iostream>

inline std::filesystem::path Worker::new_path(const std::filesystem::path &path,
                                              const std::string &name) {
  return path.parent_path() / name;
}

void Worker::sync_photos(work_type type) {}

void Worker::rename_photos(const std::string &name, work_type type) {}

void Worker::rename_folder(const std::string &name, work_type type) {
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

void Worker::read_raw_files(std::vector<std::string> &files) {
  Worker::read_files(files, raw_suffix, src);
}

void Worker::read_converted_files(std::vector<std::string> &files) {
  Worker::read_files(files, converted_suffix, dest);
}

void Worker::read_files(std::vector<std::string> &files,
                        const std::string &suffix,
                        const std::filesystem::path &path) {
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.path().extension() == suffix)
      files.push_back(entry.path().filename());
  }
}

void Worker::rename_files_in_folder(const std::filesystem::path &path,
                                    const std::string &name,
                                    const std::string &suffix, bool renumber,
                                    bool verbose, bool ask) {
  std::set<std::filesystem::path> sorted_files;
  for (const auto &file : std::filesystem::directory_iterator(path)) {
    if (file.path().extension() == suffix) {
      sorted_files.insert(file.path());
    }
  }
  if (renumber) {
    int number = 1;
    for (auto const &file : sorted_files) {
      std::filesystem::rename(
          file, Worker::new_path(file, name + std::to_string(number) + suffix));
      ++number;
    }
  } else {
    std::string new_name;
    std::string file_name;
    for (auto const &file : sorted_files) {
      file_name = file.filename();
      new_name = name + file_name.substr(file_name.find_first_of("1234567890"));
      std::filesystem::rename(file, Worker::new_path(file, new_name));
    }
  }
}