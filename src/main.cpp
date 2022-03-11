#include <cstdlib>
#include <iostream>
#include <libraw/libraw.h>
#include <string>
#include <vector>

#include "argument_parse.hpp"
#include "convertor.hpp"
#include "pictures.hpp"
#include "worker.hpp"

enum run_type { sync_files, rename_files, convert_files };

int main(int argc, char **args) {
  run_type selected;
  std::string argument = std::string(*(args + 1));

  // Process first arguments
  if (argument == "sync")
    selected = sync_files;
  else if (argument == "rename")
    selected = rename_files;
  else if (argument == "convert")
    selected = convert_files;
  else {
    std::cout << "Help:" << std::endl;
    return EXIT_FAILURE;
  }

  // Process all flags and save them to structure
  arguments arg = arguments();
  try {
    if (read_arguments(argc, args, arg)) {
      std::cout << "Fail to load arguments." << std::endl;
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }

  // Init worker
  Worker worker = Worker(arg.source, arg.destination);
  worker.verbose = arg.verbose;
  worker.ask = arg.ask;

  // Do selected option
  switch (selected) {
  case sync_files:
    worker.sync_photos(original);
    break;
  case rename_files:
    // Check if name was setted
    if (arg.name.empty()) {
      std::cout << "Name missing for rename" << std::endl;
      return EXIT_FAILURE;
    }
    worker.rename_photos(arg.name, both);
    break;
  case convert_files:
    // Read list of files for convertion
    std::vector<std::string> *aux = new std::vector<std::string>;
    worker.read_raw_files(*aux);

    // Initialize convertor
    Convertor convertor = Convertor(arg.source, arg.destination);
    convertor.verbose = arg.verbose;
    convertor.quiet = arg.quiet;

    // Convert pictures and take list of them with info
    std::shared_ptr<Pictures> pictures =
        convertor.conver_photos_list(*aux, arg.threads);

    delete aux;

    if (!arg.name.empty()) {
      worker.rename_photos(arg.name, both);
    }
    break;
  }

  return EXIT_SUCCESS;
}