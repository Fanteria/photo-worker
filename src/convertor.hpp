#ifndef CONVERTOR_H_INCLUDED
#define CONVERTOR_H_INCLUDED

#include <filesystem>
#include <iostream>
#include <libraw/libraw.h>
#include <turbojpeg.h>

#include "pictureData.hpp"
#include "pictures.hpp"

namespace fs = std::filesystem;

class Convertor {
private:
  const fs::path src;
  const fs::path dest;

  /**
   * @brief Load picture with LiwRaw.
   *
   * @param file_name is name of loaded file
   * @param iProcessor is reference for LibRaw processor
   * @return int 0 if image was loaded, other numbers represents output of
   * LibRaw open_file
   */
  int load_picture(const std::string &file_name, LibRaw &iProcessor);

  /**
   * @brief Read data about loaded picture.
   *
   * @param iProcessor is reference for LibRaw processor
   * @return PictureData* pointer to loaded picture data
   */
  PictureData *read_picture_data(LibRaw &iProcessor);

  /**
   * @brief Covert picture to jpg format.
   *
   * @param file_name is name of new file
   * @param iProcessor is reference for LibRaw processor
   */
  void convert_picture(const std::string &file_name, LibRaw &iProcessor,
                       tjhandle *compressor, unsigned char *compressedImage,
                       long unsigned int *size, int quality);

  /**
   * @brief Compress and save bitmap to jpg.
   *
   * @param mem_image
   * @param compressor
   * @param compressedImage
   * @param size
   * @param name
   * @param quality
   */
  void save_jpg(const libraw_processed_image_t *mem_image, tjhandle *compressor,
                unsigned char *compressedImage, long unsigned int *size,
                const std::string &name, int quality);

  /**
   * @brief Process picture and convert picture if convert is true.
   *
   * @param file_name is name of picture to procession
   * @param iProcessor is reference to LibRaw processor
   * @param pictures is shared pointer to class contains pictures
   * @param convert if it is true, picture will be converted
   */
  void process_picture(const std::string &file_name, LibRaw &iProcessor,
                       std::shared_ptr<Pictures> pictures, bool convert = true);

public:
  bool verbose = false;
  bool quiet = false;

  /**
   * @brief Construct a new Convertor object.
   *
   * @param src path to source folder
   * @param dest path to destination folder
   */
  Convertor(const fs::path &src, const fs::path &dest) : src(src), dest(dest) {}

  /**
   * @brief Convert all pictures in list.
   *
   * @param pics list of pictures to convert
   * @param threads number of threads for convert photos
   * @return std::shared_ptr<Pictures> pointer to class with list of loaded
   * data from pictures
   */
  std::shared_ptr<Pictures>
  conver_photos_list(const std::vector<std::string> &pics,
                     unsigned int threads = 1);
};

#endif // CONVERTOR_H_INCLUDED