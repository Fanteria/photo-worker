#include <exception>
#include <iostream>
#include <string>

#include "argument_parse.hpp"
#include "convertor.hpp"
#include "pictures.hpp"
#include "worker.hpp"

int main(int argc, char **argv) {
  try {
    Arguments arg(argc, argv);

    // Init worker
    Worker worker = Worker(arg.source, arg.destination);
    worker.verbose = arg.verbose;
    worker.ask = arg.ask;

    // Do selected option
    switch (arg.command) {
      case Arguments::SYNC_FILES:
        worker.sync_photos(Worker::ORIGINAL);
        break;
      case Arguments::RENAME_FILES:
        worker.rename_photos(arg.name, Worker::BOTH);
        break;
      case Arguments::CONVERT_FILES: {
        // Read list of files for convertion
        std::vector<std::string> aux;
        worker.read_raw_files(aux);

        // Initialize convertor
        Convertor convertor = Convertor(arg.source, arg.destination, arg.threads);
        convertor.verbose = arg.verbose;
        convertor.quiet = arg.quiet;

        // Convert pictures and take list of them with info
        std::shared_ptr<Pictures> pictures = convertor.conver_photos_list(aux);

        if (!arg.name.empty()) {
          worker.rename_photos(arg.name, Worker::BOTH);
        }
        break;
      }
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    Arguments::help(std::cout);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
