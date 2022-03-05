#include <iostream>
#include <libraw/libraw.h>
#include <string>

#include "argument_parse.h"
#include "convertor.h"

int process_image(const std::string &file, LibRaw &iProcessor);
void print_image_info(const LibRaw &iProcessor);

int main(int argc, char **args) {
  arguments *arg = new arguments();
  try {
    read_arguments(argc, args, *arg);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }

  LibRaw iProcessor;
  Convertor convertor = Convertor(arg->source, arg->destination);
 
  convertor.convert_photo("example.CR2", iProcessor);

  return EXIT_SUCCESS;
}