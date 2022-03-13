#ifndef CONVERTOR_H_INCLUDED
#define CONVERTOR_H_INCLUDED

#include <atomic>
#include <chrono>
#include <exception>
#include <execution>
#include <filesystem>
#include <iostream>
#include <libraw/libraw.h>
#include <memory>
#include <thread>
#include <turbojpeg.h>
#include <vector>

#include "pictureData.hpp"
#include "pictures.hpp"

namespace fs = std::filesystem;

class Convertor {
private:
  int quality = 75;
  const fs::path src;
  const fs::path dest;
  std::vector<LibRaw *> iProcessors;
  std::vector<tjhandle> tjCompressors;
  std::vector<unsigned char *> compImages;
  std::vector<unsigned long int> compImageSizes;

  /**
   * @brief Load picture with LiwRaw.
   *
   * @param file_name is name of loaded file
   * @param iProcessor is reference for LibRaw processor
   * @return int 0 if image was loaded, other numbers represents output of
   * LibRaw open_file
   */
  static int load_picture(const std::string &file_name, LibRaw &iProcessor);

  /**
   * @brief Read data about loaded picture.
   *
   * @param iProcessor is reference for LibRaw processor
   * @return PictureData* pointer to loaded picture data
   */
  static PictureData *read_picture_data(LibRaw &iProcessor);

  /**
   * @brief This method reset all buffer after processing all pictures. Does not
   * have to be called after every processed picture.
   *
   */
  void reset_buffers();

  /**
   * @brief Covert picture to jpg format.
   *
   * @param file_name is name of new file
   * @param procNum number of processor, convertor and buffer for this method
   */
  void convert_picture(const std::string &file_name, size_t procNum);

  /**
   * @brief Compress and save bitmap to jpeg image.
   *
   * @param mem_image memory with bitmap image
   * @param procNum number of processor, convertor and buffer for this method
   * @param name is name of saved file
   */
  void save_jpg(const libraw_processed_image_t *mem_image, size_t procNum,
                const std::string &name);

  /**
   * @brief Process picture and convert picture if convert is true.
   *
   * @param file_name is name of picture to procession
   * @param procNum number of processor, convertor and buffer for this method
   * @param pictures is shared pointer to class contains pictures
   * @param convert if it is true, picture will be converted
   */
  void process_picture(const std::string &file_name, size_t procNum,
                       std::shared_ptr<Pictures> pictures, bool convert = true);

  // TODO doxygen
  void process_list(size_t procNum, std::atomic<size_t> *index,
                    const std::vector<std::string> *pics,
                    std::shared_ptr<Pictures> picList);

  // TODO doxygen
  static std::string get_info_string(size_t &last, size_t act, size_t max,
                                     const std::vector<std::string> &pics,
                                     bool verbose);

  // TODO doxygen
  static void print_info(std::atomic<size_t> *index,
                         const std::vector<std::string> *pics, size_t threadNum,
                         bool verbose);

public:
  bool verbose = false;
  bool quiet = false;

  /**
   * @brief Construct a new Convertor object.
   *
   * @param src path to source folder
   * @param dest path to destination folder
   * @param threads number of treads available for compressing images minimum is
   * 1, does not represents total number of threads
   */
  Convertor(const fs::path &src, const fs::path &dest, size_t threads = 1);

  /**
   * @brief Destroy the Convertor object
   *
   */
  ~Convertor();

  /**
   * @brief Set the quality of picture. Value must be between 1 and 100
   * included. If not, function throw std::invalid_argument exception.
   *
   * @param quality is quality of compressed image.
   */
  void set_quality(unsigned int quality);

  /**
   * @brief Convert all pictures in list.
   *
   * @param pics list of pictures to convert
   * @return std::shared_ptr<Pictures> pointer to class with list of loaded
   * data from pictures
   */
  std::shared_ptr<Pictures>
  conver_photos_list(const std::vector<std::string> &pics);
};

#endif // CONVERTOR_H_INCLUDED