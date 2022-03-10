#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace fs = std::filesystem;

enum work_type { original, converted, both };

class Worker {
private:
  const fs::path src;
  const fs::path dest;

  /**
   * @brief Create path with new name of last file.
   *
   * @param path is path to file
   * @param name is new name
   * @return fs::path is path with new name
   */
  static fs::path new_path(const fs::path &path, const std::string &name);

  /**
   * @brief Get the sorted set of files included in directory.
   *
   * @param path is path to parental directory
   * @param suffix is pointer suffix of relevant files, if it's null, then all
   * files will be sorted
   * @return std::set<fs::path> set of sorted files
   */
  static std::set<fs::path> get_sorted_files(const fs::path &path,
                                             const std::string *suffix);

  /**
   * @brief Remove all files thats in synced folder and not in original folder.
   *
   * @param original folder that contains files for sync
   * @param synced folder with files thats will be removed
   */
  static void remove_surplus_files(const fs::path &original,
                                   const fs::path &synced);

public:
  std::string raw_suffix = ".CR2";
  std::string converted_suffix = ".jpg";
  bool verbose = false;
  bool ask = false;

  /**
   * @brief Construct a new Worker object.
   *
   * @param src is folder with raw pictures
   * @param dest is folder with converted pictures
   */
  Worker(const fs::path &src, const fs::path &dest) : src(src), dest(dest) {}

  /**
   * @brief Method for sync pictures in folders.
   *
   * @param type id type of conversion, can by removed files in source folder,
   * destination folder or in both
   */
  void sync_photos(work_type type);

  /**
   * @brief Method for rename all photos in folder.
   *
   * @param name is new name for files
   * @param type to decide in which folder will be files renamed, source,
   * destination or both
   */
  void rename_photos(const std::string &name, work_type type);

  /**
   * @brief Rename folder contains pictures.
   *
   * @param name is new name of folder
   * @param type to decide which folder will be renamed, source, destination or
   * both
   */
  void rename_folder(const std::string &name, work_type type = original);

  /**
   * @brief Add all raw files to vector in argument.
   *
   * @param files is vector to add raw files
   */
  void read_raw_files(std::vector<std::string> &files);

  /**
   * @brief Add all converted files to vector in argument.
   *
   * @param files is vector to add converted files
   */
  void read_converted_files(std::vector<std::string> &files);

  /**
   * @brief Get the name of file without path and suffix
   *
   * @param path is path to file
   * @return std::string is name of file in path
   */
  static std::string get_filename(const fs::path &path);

  /**
   * @brief Add all files with specified suffix to vector in argument.
   *
   * @param files is vector to add files
   * @param suffix is suffix of added files
   * @param path is path to directory with files
   */
  static void read_files(std::vector<std::string> &files,
                         const std::string &suffix, const fs::path &path);

  /**
   * @brief Method fo rename all files with specified suffix in specified
   * folder.
   *
   * @param path is path to direcotry with files
   * @param name is new name for files
   * @param suffix is suffix of renamed files
   * @param renumber if it is true, renamed files will be numbered from 1, if
   * false, numbers of files satay same
   * @param verbose if it is true, output will be verbose
   * @param ask if it is true, ask for every file while renamig
   */
  static void rename_files_in_folder(const fs::path &path,
                                     const std::string &name,
                                     const std::string &suffix, bool renumber,
                                     bool verbose, bool ask);
};

#endif // WORKER_H_INCLUDED