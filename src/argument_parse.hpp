#include <filesystem>
#include <getopt.h>
#include <stdexcept>
#include <string>

struct arguments {
  std::filesystem::path source = ".";
  std::filesystem::path destination = ".";
  unsigned int threads = 1;
  bool quiet = false;
  bool verbose = false;
  bool ask = false;
  std::string name = "";
};

/**
 * @brief Read arguments and parse them to struct.
 *
 * @param argc is argument count
 * @param argv is argument vector
 * @param arg reference to structure for parse flags
 * @return int if 1, then arguments was read right
 */
int read_arguments(int argc, char **argv, arguments &arg);

/**
 * @brief Parse string to path and check if it exist, if not, and exist parent,
 * can be created depends on argument setup. If folder not exist (and
 * have not parent), then exception is thrown.
 *
 * @param path is string representation of path
 * @param must_exist if it is true, can create folder, if exist parent
 * @return std::filesystem::path created path
 */
std::filesystem::path parse_to_path(const std::string &path,
                                    bool must_exist = true);

/**
 * @brief Parse string to unsigned int. If cannot be parsed, then throw
 * exception.
 *
 * @param number is number ins string to parse
 * @return unsigned int is parsed number
 */
unsigned int parse_to_unsigned(const std::string &number);