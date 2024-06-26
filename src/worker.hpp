#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <filesystem>
#include <set>
#include <string>
#include <vector>

class Worker {
 public:
  enum WorkType { ORIGINAL, CONVERTED, BOTH };

 private:
  const std::filesystem::path src;
  const std::filesystem::path dest;

  /**
   * @brief Create path with new name of last file.
   *
   * @param path is path to file
   * @param name is new name
   * @return fs::path is path with new name
   */
  static std::filesystem::path new_path(
      const std::filesystem::path &path, const std::string &name) noexcept;

  /**
   * @brief Get the sorted set of files included in directory.
   *
   * @param path is path to parental directory
   * @param suffix is pointer suffix of relevant files, if it's null, then all
   * files will be sorted
   * @return std::set<fs::path> set of sorted files
   */
  static std::set<std::filesystem::path> get_sorted_files(
      const std::filesystem::path &path, const std::string *suffix) noexcept;

  /**
   * @brief Remove all files thats in synced folder and not in original folder.
   *
   * @param original folder that contains files for sync
   * @param synced folder with files thats will be removed
   */
  static void remove_surplus_files(
      const std::filesystem::path &original,
      const std::filesystem::path &synced) noexcept;

 public:
  std::string raw_suffix = ".CR2";
  std::string converted_suffix = ".jpg";
  bool verbose = false;
  bool ask = false;

  /**
   * @brief Construct a new Worker object.
   *
   * @param src_ is folder with raw pictures
   * @param dest_ is folder with converted pictures
   */
  Worker(
      const std::filesystem::path &src_,
      const std::filesystem::path &dest_) noexcept;

  /**
   * @brief Method for sync pictures in folders.
   *
   * @param type id type of conversion, can by removed files in source folder,
   * destination folder or in both
   */
  void sync_photos(WorkType type) noexcept;

  /**
   * @brief Method for rename all photos in folder.
   *
   * @param name is new name for files
   * @param type to decide in which folder will be files renamed, source,
   * destination or both
   */
  void rename_photos(const std::string &name, WorkType type) noexcept;

  /**
   * @brief Rename folder contains pictures.
   *
   * @param name is new name of folder
   * @param type to decide which folder will be renamed, source, destination or
   * both
   */
  void rename_folder(const std::string &name, WorkType type = ORIGINAL) noexcept;

  /**
   * @brief Add all raw files to vector in argument.
   *
   * @param files is vector to add raw files
   */
  void read_raw_files(std::vector<std::string> &files) noexcept;

  /**
   * @brief Add all converted files to vector in argument.
   *
   * @param files is vector to add converted files
   */
  void read_converted_files(std::vector<std::string> &files) noexcept;

  /**
   * @brief Get the name of file without path and suffix
   *
   * @param path is path to file
   * @return std::string is name of file in path
   */
  static std::string get_filename(const std::filesystem::path &path) noexcept;

  /**
   * @brief Add all files with specified suffix to vector in argument.
   *
   * @param files is vector to add files
   * @param suffix is suffix of added files
   * @param path is path to directory with files
   */
  static void read_files(
      std::vector<std::string> &files,
      const std::string &suffix,
      const std::filesystem::path &path) noexcept;

  /**
   * @brief Method fo rename all files with specified suffix in specified
   * folder.
   *
   * @param path is path to direcotry with files
   * @param name is new name for files
   * @param suffix is suffix of renamed files
   * @param renumber if it is true, renamed files will be numbered from 1, if
   * false, numbers of files satay same
   */
  static void rename_files_in_folder(
      const std::filesystem::path &path,
      const std::string &name,
      const std::string &suffix,
      bool renumber) noexcept;
};

#endif  // WORKER_H_INCLUDED
