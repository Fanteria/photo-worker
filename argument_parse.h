#include <getopt.h>
#include <string>

struct arguments {
  std::string source;
  std::string destination;
  size_t threats;
  bool quiet;
};

int read_arguments(arguments &);