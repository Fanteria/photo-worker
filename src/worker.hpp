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

  static fs::path new_path(const fs::path &, const std::string &);
  static std::set<fs::path> get_sorted_files(const fs::path &,
                                             const std::string *);
  static void remove_surplus_files(const fs::path &, const fs::path &);

public:
  std::string raw_suffix = ".CR2";
  std::string converted_suffix = ".jpg";
  bool verbose = false;
  bool ask = false;

  Worker(const fs::path &src, const fs::path &dest) : src(src), dest(dest) {}

  void sync_photos(work_type);

  void rename_photos(const std::string &, work_type);

  void rename_folder(const std::string &, work_type type = original);

  void read_raw_files(std::vector<std::string> &);

  void read_converted_files(std::vector<std::string> &);

  static std::string get_filename(const fs::path &);

  static void read_files(std::vector<std::string> &, const std::string &,
                         const fs::path &);

  static void rename_files_in_folder(const fs::path &, const std::string &,
                                     const std::string &, bool, bool, bool);
};

#endif // WORKER_H_INCLUDED