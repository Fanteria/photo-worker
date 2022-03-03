#include "argument_parse.h"

#include <getopt.h>
#include <string>

int read_arguments(arguments &arg) {
  struct option long_options[] = {
      {"source", required_argument, nullptr, 's'},
      {"destination", optional_argument, nullptr, 'd'},
      {"threats", optional_argument, nullptr, 't'},
      {"quiet", no_argument, nullptr, 'q'},
      {nullptr, 0, nullptr, 0}};

  return 1;
}