#include "argument_parse.hpp"

#include <stdexcept>

// If version is not set by Makefile, use dirty
#ifndef VERSION
#define VERSION "dirty"
#endif

#ifndef PROJECT_NAME
#define PROJECT_NAME "unknown"
#endif

namespace fs = std::filesystem;

using std::invalid_argument;
using std::string;

fs::path Arguments::parse_to_path(const string &path, bool must_exist) {
  fs::path p(path);
  if (!fs::exists(p)) {
    if (must_exist) throw invalid_argument(path + " does not exist.");
    if (fs::exists(p.parent_path())) {
      // if parent exist create directory
      if (fs::create_directory(p)) {
        return p;
      } else {
        throw invalid_argument(path + " cannot create directory.");
      }
    }
  }
  if (!fs::is_directory(p)) {
    throw invalid_argument(path + "is not directory.");
  }
  return p;
}

Arguments::Arguments(int argc, char **argv) {
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

  if (argc < 2) {
    throw std::invalid_argument("command does not set");
  }

  int c;
  string path;
  bool dest_setted = false;

  const std::string command_name(argv[1]);
  if (command_name == "sync") {
    command = SYNC_FILES;
  } else if (command_name == "rename") {
    command = RENAME_FILES;
  } else if (command_name == "convert") {
    command = CONVERT_FILES;
  } else {
    throw std::invalid_argument(command_name + " is invalid command");
  }

  // Load all flags
  while ((c = getopt_long(argc, argv, "s:d:t:q", long_options, nullptr)) != -1) {
    switch (c) {
      case 's':
        source = parse_to_path(optarg);
        if (!dest_setted) destination = parse_to_path(source / "jpg", false);
        break;
      case 'd':
        destination = parse_to_path(optarg, false);
        dest_setted = true;
        break;
      case 't':
        threads = std::stoul(optarg);
        break;
      case 'q':
        quiet = true;
        break;
      case 'v':
        verbose = true;
        break;
      case 'a':
        ask = true;
        break;
      case 'n':
        name = optarg;
        break;
      default:
        throw std::invalid_argument("unknown flag");
    }
  }

  // Check if arguments are valid
  if (command == RENAME_FILES && name.empty()) {
    throw std::invalid_argument("Name for rename must be set.");
  }
}

void Arguments::help(std::ostream &os) noexcept {
  os << "Usage: " PROJECT_NAME "COMMAND [OPTIONS]\n\n";
  os << "Version: " VERSION "\n";
  os << "This project implements the conversion from raw format to jpeg"
        "images and enable some basic photo movements.\n\n"
        "Commands:\n"
        "  sync   \tSynchronize files between two folders independent of the extension.\n"
        "  rename \tRename and renumber files in given directory.\n"
        "  convert\tCovert raw files to jpeg.\n"
        "\n"
        "Options:\n"
        "  -s, --source     \tSource directory. Default is `.`.\n"
        "  -d, --destination\tDestination directory. Default is `.`.\n"
        "  -t, --threads    \tNumber of threads used to work. Default is 1.\n"
        "  -q, --quiet      \tMake run quiet.\n"
        "  -v, --verbose    \tMake run more verbose.\n"
        "  -a, --ask        \tAsk for changes to made.\n"
        "  -n, --name       \tNon empty name for renaming.\n";
}
