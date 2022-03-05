#include <iostream>
#include <libraw/libraw.h>
#include <string>

#include "argument_parse.h"
#include "convertor.h"

int main(int argc, char **args) {
  arguments arg = arguments();
  try {
    read_arguments(argc, args, arg);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }

  LibRaw iProcessor;
  Convertor convertor = Convertor(arg.source, arg.destination);
 
  convertor.convert_photo("example.CR2", iProcessor);

  return EXIT_SUCCESS;
}