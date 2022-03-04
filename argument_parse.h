#include <filesystem>
#include <getopt.h>
#include <string>

struct arguments {
  std::filesystem::path source = ".";
  std::filesystem::path destination = ".";
  unsigned int threads = 1;
  bool quiet = false;
  bool verbose = false;
  bool ask = false;
};

int read_arguments(int, char **, arguments &);

std::filesystem::path parse_to_path(const std::string &);

unsigned int parse_to_unsigned(const std::string &);