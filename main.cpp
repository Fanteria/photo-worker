#include <iostream>
#include <libraw/libraw.h>
#include <string>
#include <vector>

#include "argument_parse.h"
#include "convertor.h"
#include "worker.h"

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
  for (auto item : *aux) {
    std::cout << item << std::endl;
  }

  std::cout << std::endl;

  LibRaw iProcessor;
  Convertor convertor = Convertor(arg.source, arg.destination);
  convertor.conver_photos_list(*aux, arg.threads);

  delete aux;

  return EXIT_SUCCESS;
}