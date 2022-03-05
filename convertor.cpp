#include "convertor.h"

int Convertor::convert_photo(const std::string & file, LibRaw & iProcessor) {
    int ret;
    if ((ret = iProcessor.open_file(file.c_str())) != LIBRAW_SUCCESS) {
        std::cerr << "Error: " << libraw_strerror(ret) << std::endl;
        return ret;
    }

    iProcessor.unpack();

    this->print_image_info(iProcessor);

    return 0;
}

void Convertor::print_image_info(const LibRaw & iProcessor) {
    std::cout << "Image size:\t";
    std::cout << iProcessor.imgdata.sizes.width;
    std::cout << " x ";
    std::cout << iProcessor.imgdata.sizes.height;
    std::cout << "\t\tWithout frame";
    std::cout << std::endl;

    std::cout << "Output image:\t";
    std::cout << iProcessor.imgdata.sizes.iwidth;
    std::cout << " x ";
    std::cout << iProcessor.imgdata.sizes.iheight;
    std::cout << "\t\tSize of output image, can be different";
    std::cout << std::endl;

    std::cout << "Margin size:\t";
    std::cout << iProcessor.imgdata.sizes.top_margin;
    std::cout << " x ";
    std::cout << iProcessor.imgdata.sizes.left_margin;
    std::cout << "\t\tCoordinates of left top corner shift";
    std::cout << std::endl;

    std::cout << "Raw image size:\t";
    std::cout << iProcessor.imgdata.sizes.raw_width;
    std::cout << " x ";
    std::cout << iProcessor.imgdata.sizes.raw_height;
    std::cout << "\t\tWith frame";
    std::cout << std::endl;
}