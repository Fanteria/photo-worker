#include "argument_parse.hpp"
#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>

int read_arguments(int argc, char **argv, arguments &arg) {
  // Define possible flags for getopt
  struct option long_options[] = {
      {"source", required_argument, nullptr, 's'},
      {"destination", required_argument, nullptr, 'd'},
      {"threads", required_argument, nullptr, 't'},
      {"quiet", no_argument, nullptr, 'q'},
      {"verbose", no_argument, nullptr, 'v'},
      {"ask", no_argument, nullptr, 'a'},
      {"name", required_argument, nullptr, 'n'},
      {nullptr, 0, nullptr, 0}};

  char c;
  std::string path;
  bool dest_setted = false;

  // Load all flags
  while ((c = getopt_long(argc, argv, "s:d:t:q", long_options, NULL)) != -1) {
    switch (c) {
    case 's':
      arg.source = parse_to_path(optarg);
      if (!dest_setted)
        arg.destination = parse_to_path(arg.source / "jpg", false);
      break;
    case 'd':
      arg.destination = parse_to_path(optarg, false);
      dest_setted = true;
      break;
    case 't':
      arg.threads = parse_to_unsigned(optarg);
      break;
    case 'q':
      arg.quiet = true;
      break;
    case 'v':
      arg.verbose = true;
      break;
    case 'a':
      arg.ask = true;
      break;
    case 'n':
      arg.name = optarg;
      break;
    default:
      return 1;
      break;
    }
  }

  return 0;
}

fs::path parse_to_path(const std::string &path, bool must_exist) {
  fs::path p(path);
  if (!fs::exists(p)) {
    if (must_exist)
      throw std::invalid_argument(path + " does not exist.");
    if (fs::exists(p.parent_path())) {
      // if parent exist create directory
      if (fs::create_directory(p)) {
        return p;
      } else {
        throw std::invalid_argument(path + " cannot create directory.");
      }
    }
  }
  if (!fs::is_directory(p)) {
    throw std::invalid_argument(path + "is not directory.");
  }
  return p;
}

unsigned int parse_to_unsigned(const std::string &number) {
  int threads;
  try {
    threads = std::stoi(number);
  } catch (std::invalid_argument &e) {
    throw std::invalid_argument("Number of threads must be number.");
  }
  if (threads <= 0)
    throw std::invalid_argument("Number of threads must be positive.");
  return (unsigned int)threads;
}