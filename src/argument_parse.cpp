#include "argument_parse.hpp"


int read_arguments(int argc, char **argv, arguments &arg) {
  struct option long_options[] = {
      {"source", required_argument, nullptr, 's'},
      {"destination", required_argument, nullptr, 'd'},
      {"threads", required_argument, nullptr, 't'},
      {"quiet", no_argument, nullptr, 'q'},
      {"verbose", no_argument, nullptr, 'v'},
      {"ask", no_argument, nullptr, 'a'},
      {nullptr, 0, nullptr, 0}};

  char c;
  std::string path;
  while ((c = getopt_long(argc, argv, "s:d:t:q", long_options, NULL)) != -1) {
    switch (c) {
    case 's':
      arg.source = parse_to_path(optarg);
      break;
    case 'd':
      arg.destination = parse_to_path(optarg);
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
    default:
      break;
    }
  }

  return 0;
}

std::filesystem::path parse_to_path(const std::string &path) {
  std::filesystem::path p(path);
  if (!std::filesystem::exists(p))
    throw std::invalid_argument(path + " does not exist.");
  if (!std::filesystem::is_directory(p))
    throw std::invalid_argument(path + "is not directory.");
  return p;
}

unsigned int parse_to_unsigned(const std::string & number) {
  int threads;
  try {
    threads = std::stoi(number);
  } catch (std::invalid_argument &e) {
    throw std::invalid_argument("Number of threads must be number.");
  }
  if (threads <= 0)
    throw std::invalid_argument("Number of threads must be positive.");
  return (unsigned int) threads;
}