#include "pictureData.hpp"

using std::make_pair;
using std::ostream, std::endl;
using std::pair;

pair<unsigned int, unsigned int> PictureData::get_image_size() {
  return make_pair(this->width, this->height);
}

pair<unsigned int, unsigned int> PictureData::get_output_image_size() {
  return make_pair(this->out_width, this->out_height);
}

pair<unsigned int, unsigned int> PictureData::get_margin_size() {
  return make_pair(this->top_margin, this->left_margin);
}

pair<unsigned int, unsigned int> PictureData::get_raw_image_size() {
  return make_pair(this->raw_width, this->raw_height);
}

ostream &operator<<(ostream &os, const PictureData &pic) {
  os << "Image size:\t";
  os << pic.width;
  os << " x ";
  os << pic.height;
  os << "\t\tWithout frame\n";

  os << "Output image:\t";
  os << pic.out_width;
  os << " x ";
  os << pic.out_height;
  os << "\t\tSize of output image, can be different\n";

  os << "Margin size:\t";
  os << pic.top_margin;
  os << " x ";
  os << pic.left_margin;
  os << "\t\tCoordinates of left top corner shift\n";

  os << "Raw image size:\t";
  os << pic.raw_width;
  os << " x ";
  os << pic.raw_height;
  os << "\t\tWith frame";
  os << endl;

  return os;
}