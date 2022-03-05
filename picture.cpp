#include "picture.h"

std::pair<unsigned int, unsigned int> Picture::get_image_size() {
    return std::make_pair(this->width, this->height);
}

std::pair<unsigned int, unsigned int> Picture::get_output_image_size() {
    return std::make_pair(this->out_width, this->out_height);
}

std::pair<unsigned int, unsigned int> Picture::get_margin_size() {
    return std::make_pair(this->top_margin, this->left_margin);
}

std::pair<unsigned int, unsigned int> Picture::get_raw_image_size() {
    return std::make_pair(this->raw_width, this->raw_height);
}

std::ostream& operator<< (std::ostream & os, const Picture & pic) {
    std::cout << "Image size:\t";
    std::cout << pic.width;
    std::cout << " x ";
    std::cout << pic.height;
    std::cout << "\t\tWithout frame";
    std::cout << std::endl;

    std::cout << "Output image:\t";
    std::cout << pic.out_width;
    std::cout << " x ";
    std::cout << pic.out_height;
    std::cout << "\t\tSize of output image, can be different";
    std::cout << std::endl;

    std::cout << "Margin size:\t";
    std::cout << pic.top_margin;
    std::cout << " x ";
    std::cout << pic.left_margin;
    std::cout << "\t\tCoordinates of left top corner shift";
    std::cout << std::endl;

    std::cout << "Raw image size:\t";
    std::cout << pic.raw_width;
    std::cout << " x ";
    std::cout << pic.raw_height;
    std::cout << "\t\tWith frame";
    std::cout << std::endl;

    return os;
}