#include <filesystem>
#include <getopt.h>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

struct arguments {
  fs::path source = ".";
  fs::path destination = ".";
  unsigned int threads = 1;
  bool quiet = false;
  bool verbose = false;
  bool ask = false;
  std::string name = "";
};

int read_arguments(int, char **, arguments &);

std::filesystem::path parse_to_path(const std::string &,
                                    bool must_exist = true);

unsigned int parse_to_unsigned(const std::string &);