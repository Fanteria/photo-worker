#include "pictureData.hpp"

using std::ostream, std::endl;
using std::pair;

PictureData::PictureData(const unsigned int width_, const unsigned int height_,
            const unsigned int out_width_, const unsigned int out_height_,
            const unsigned int top_margin_, const unsigned int left_margin_,
            const unsigned int raw_width_, const unsigned int raw_height_) noexcept
    : width(width_), height(height_), out_width(out_width_),
      out_height(out_height_), top_margin(top_margin_),
      left_margin(left_margin_), raw_width(raw_width_), raw_height(raw_height_) {
}

pair<unsigned int, unsigned int> PictureData::get_image_size() noexcept {
  return {width, height};
}

pair<unsigned int, unsigned int> PictureData::get_output_image_size() noexcept {
  return {out_width, out_height};
}

pair<unsigned int, unsigned int> PictureData::get_margin_size() noexcept {
  return {top_margin, left_margin};
}

pair<unsigned int, unsigned int> PictureData::get_raw_image_size() noexcept {
  return {raw_width, raw_height};
}

ostream &operator<<(ostream &os, const PictureData &pic) noexcept {
  os << "Image size:\t" << pic.width << " x " << pic.height << "\t\tWithout frame\n";

  os << "Output image:\t" << pic.out_width << " x " << pic.out_height
     << "\t\tSize of output image, can be different\n";

  os << "Margin size:\t" << pic.top_margin << " x " << pic.left_margin
     << "\t\tCoordinates of left top corner shift\n";

  os << "Raw image size:\t" << pic.raw_width << " x " << pic.raw_height
     << "\t\tWith frame\n";

  return os;
}
