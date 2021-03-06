#include "worker.hpp"

namespace fs = std::filesystem;

using std::set, std::map, std::pair, std::vector;
using std::string;

inline fs::path Worker::new_path(const fs::path &path, const string &name) {
  return path.parent_path() / name;
}

inline set<fs::path> Worker::get_sorted_files(const fs::path &path,
                                              const string *suffix) {
  set<fs::path> sorted_files;
  for (const auto &file : fs::directory_iterator(path)) {
    if (suffix && file.path().extension() != *suffix)
      continue;
    sorted_files.insert(file.path());
  }
  return sorted_files;
}

void Worker::remove_surplus_files(const fs::path &original,
                                  const fs::path &synced) {
  map<string, fs::path> files_set;
  // Load all files (without folders) from synced folder
  for (const auto &file : fs::directory_iterator(synced)) {
    if (file.is_regular_file())
      files_set.insert(pair<string, fs::path>(get_filename(file), file));
  }
  // Remove from loaded files all thats exist in original folder
  for (const auto &file : fs::directory_iterator(original)) {
    files_set.erase(get_filename(file));
  }
  // Delete files not removed in previous step
  for (const auto &file : files_set) {
    fs::remove(file.second);
  }
}

void Worker::sync_photos(work_type type) {
  switch (type) {
  case original:
    remove_surplus_files(src, dest);
    break;
  case both:
    remove_surplus_files(src, dest);
  case converted:
    remove_surplus_files(dest, src);
    break;
  }
  remove_surplus_files(src, dest);
}

void Worker::rename_photos(const string &name, work_type type) {
  switch (type) {
  case original:
    rename_files_in_folder(src, name, raw_suffix, false, verbose, ask);
    break;
  case both:
    rename_files_in_folder(src, name, raw_suffix, false, verbose, ask);
  case converted:
    rename_files_in_folder(dest, name, converted_suffix, false, verbose, ask);
    break;
  }
}

void Worker::rename_folder(const string &name, work_type type) {
  switch (type) {
  case original:
    fs::rename(src, Worker::new_path(src, name));
    break;
  case both:
    fs::rename(src, Worker::new_path(src, name));
  case converted:
    fs::rename(dest, Worker::new_path(dest, name));
    break;
  }
}

void Worker::read_raw_files(vector<string> &files) {
  Worker::read_files(files, raw_suffix, src);
}

void Worker::read_converted_files(vector<string> &files) {
  Worker::read_files(files, converted_suffix, dest);
}

inline string Worker::get_filename(const fs::path &path) {
  string file_name = string(path.filename());
  return file_name.substr(0, file_name.find_last_of('.'));
}

void Worker::read_files(vector<string> &files, const string &suffix,
                        const fs::path &path) {
  for (const auto &entry : fs::directory_iterator(path)) {
    if (entry.path().extension() == suffix)
      files.push_back(entry.path().filename());
  }
}

void Worker::rename_files_in_folder(const fs::path &path, const string &name,
                                    const string &suffix, bool renumber,
                                    bool verbose, bool ask) {
  set<fs::path> sorted_files = Worker::get_sorted_files(path, &suffix);
  if (renumber) {
    int number = 1;
    for (auto const &file : sorted_files) {
      fs::rename(
          file, Worker::new_path(file, name + std::to_string(number) + suffix));
      ++number;
    }
  } else {
    string new_name;
    string file_name;
    for (auto const &file : sorted_files) {
      file_name = file.filename();
      new_name = name + file_name.substr(file_name.find_first_of("1234567890"));
      fs::rename(file, Worker::new_path(file, new_name));
    }
  }
}