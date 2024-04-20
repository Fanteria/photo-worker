#ifndef PICTURE_DATA_H_INCLUDED
#define PICTURE_DATA_H_INCLUDED

#include <iostream>

class PictureData {
private:
  // Image size without frame
  const unsigned int width;
  const unsigned int height;

  // Output image size
  const unsigned int out_width;
  const unsigned int out_height;

  // Margin size for shift from left corner
  const unsigned int top_margin;
  const unsigned int left_margin;

  // Raw size with frame
  const unsigned int raw_width;
  const unsigned int raw_height;

public:
  /**
   * @brief Construct a new Picture Data object.
   *
   * @param width image width without frame
   * @param height image height without frame
   * @param out_width output image width
   * @param out_height output image height
   * @param top_margin margin size on top of image
   * @param left_margin margin size of left of image
   * @param raw_width raw image width
   * @param raw_height raw image height
   */
  PictureData(const unsigned int width, const unsigned int height,
              const unsigned int out_width, const unsigned int out_height,
              const unsigned int top_margin, const unsigned int left_margin,
              const unsigned int raw_width, const unsigned int raw_height) noexcept;

  /**
   * @brief Get size of the image.
   *
   * @return std::pair<unsigned int, unsigned int> size of image as <width,
   * height>
   */
  std::pair<unsigned int, unsigned int> get_image_size() noexcept;

  /**
   * @brief Get size of the output image.
   *
   * @return std::pair<unsigned int, unsigned int> size of output image as
   * <width, height>
   */
  std::pair<unsigned int, unsigned int> get_output_image_size() noexcept;

  /**
   * @brief Get the margin size of the picture.
   *
   * @return std::pair<unsigned int, unsigned int> size of margin of the picture
   * as <top, left>
   */
  std::pair<unsigned int, unsigned int> get_margin_size() noexcept;

  /**
   * @brief Get the size of raw image.
   *
   * @return std::pair<unsigned int, unsigned int> size of the raw image ad
   * <width, height>
   */
  std::pair<unsigned int, unsigned int> get_raw_image_size() noexcept;

  /**
   * @brief Overload operator <<.
   */
  friend std::ostream &operator<<(std::ostream &os, const PictureData &pic) noexcept;
};

#endif // PICTURE_DATA_H_INCLUDED
