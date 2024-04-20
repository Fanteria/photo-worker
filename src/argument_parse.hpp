#include <getopt.h>

#include <filesystem>
#include <string>

/*
 * Parse command line arguments.
 */
class Arguments {
 public:
  enum RunType {
    SYNC_FILES,
    RENAME_FILES,
    CONVERT_FILES,
  };

 private:
  /**
   * @brief Parse string to path and check if it exist, if not, and exist parent,
   * can be created depends on argument setup. If folder not exist (and
   * have not parent), then exception is thrown.
   *
   * @exception invalid_argument Throws if file is not a directory, directory
   * cannot be created or if is set `must_exist` and path does not exists.
   *
   * @param path is string representation of path
   * @param must_exist if it is true, can create folder, if exist parent
   * @return std::filesystem::path created path
   */
  std::filesystem::path parse_to_path(
      const std::string& path, bool must_exist = true);

 public:
  RunType command;
  std::filesystem::path source = ".";
  std::filesystem::path destination = ".";
  unsigned long threads = 1;
  bool quiet = false;
  bool verbose = false;
  bool ask = false;
  std::string name = "";

  /**
   * @brief Read arguments and parse them to struct.
   *
   * @exception invalid_argument Throws if anything in parsing goes wrong.
   * `exception::what()` contain reason.
   *
   * @param argc is argument count
   * @param argv is argument vector
   */
  Arguments(int argc, char** argv);

  /**
   * @brief Print help to ostream.
   *
   * @param os ostream to use as output.
   */
  static void help(std::ostream& os) noexcept;
};
