#include <iostream>
#include <libraw/libraw.h>
#include <string>
#include <vector>

#include "argument_parse.hpp"
#include "convertor.hpp"
#include "worker.hpp"
#include "pictures.hpp"

int main(int argc, char **args) {
  arguments arg = arguments();
  try {
    read_arguments(argc, args, arg);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }

  std::vector<std::string> *aux = new std::vector<std::string>;

  Worker worker = Worker(arg.source, arg.destination);
  worker.read_raw_files(*aux);

  Convertor convertor = Convertor(arg.source, arg.destination);
  std::shared_ptr<Pictures> pictures = convertor.conver_photos_list(*aux, arg.threads);

  std::cout << "List pictures: " << std::endl;
  std::cout << pictures->listAll() << std::endl;

  delete aux;

  /*
  try {
    worker.rename_folder("hello", converted);
  } catch (const std::filesystem::filesystem_error & e) {
    std::cout << e.what() << std::endl;
  }
  */

  return EXIT_SUCCESS;
}