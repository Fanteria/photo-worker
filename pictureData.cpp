#include "picture.h"

std::pair<unsigned int, unsigned int> PictureData::get_image_size() {
    return std::make_pair(this->width, this->height);
}

std::pair<unsigned int, unsigned int> PictureData::get_output_image_size() {
    return std::make_pair(this->out_width, this->out_height);
}

std::pair<unsigned int, unsigned int> PictureData::get_margin_size() {
    return std::make_pair(this->top_margin, this->left_margin);
}

std::pair<unsigned int, unsigned int> PictureData::get_raw_image_size() {
    return std::make_pair(this->raw_width, this->raw_height);
}

std::ostream& operator<< (std::ostream & os, const PictureData & pic) {
    os << "Image size:\t";
    os << pic.width;
    os << " x ";
    os << pic.height;
    os << "\t\tWithout frame";
    os << std::endl;

    os << "Output image:\t";
    os << pic.out_width;
    os << " x ";
    os << pic.out_height;
    os << "\t\tSize of output image, can be different";
    os << std::endl;

    os << "Margin size:\t";
    os << pic.top_margin;
    os << " x ";
    os << pic.left_margin;
    os << "\t\tCoordinates of left top corner shift";
    os << std::endl;

    os << "Raw image size:\t";
    os << pic.raw_width;
    os << " x ";
    os << pic.raw_height;
    os << "\t\tWith frame";
    os << std::endl;

    return os;
}