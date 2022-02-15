#include <iostream>
#include <string>
#include <libraw/libraw.h>

int process_image(const std::string & file, LibRaw & iProcessor);
void print_image_info(const LibRaw & iProcessor);

int main(int argc, char ** args) {
    LibRaw iProcessor;
    process_image("example.CR2", iProcessor);
    return 0;
}

int process_image(const std::string & file, LibRaw & iProcessor) {
    int ret;
    if ((ret = iProcessor.open_file(file.c_str())) != LIBRAW_SUCCESS) {
        std::cerr << "Error: " << libraw_strerror(ret) << std::endl;
        return ret;
    }

    iProcessor.unpack();
    
    print_image_info(iProcessor);

    return 0; 
     
}

void print_image_info(const LibRaw & iProcessor) {
    std::cout << "Image size:\t";
    std::cout << iProcessor.imgdata.sizes.width;
    std::cout << " x ";
    std::cout << iProcessor.imgdata.sizes.height;
    std::cout << "\t\tWithout frame";
    std::cout << std::endl;

    std::cout << "Raw image size:\t";
    std::cout << iProcessor.imgdata.sizes.raw_width;
    std::cout << " x ";
    std::cout << iProcessor.imgdata.sizes.raw_height;
    std::cout << "\t\tWith frame";
    std::cout << std::endl;
}