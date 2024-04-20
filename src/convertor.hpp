#ifndef CONVERTOR_H_INCLUDED
#define CONVERTOR_H_INCLUDED

#include <atomic>
#include <filesystem>
#include <libraw/libraw.h>
#include <memory>
#include <turbojpeg.h>
#include <vector>

#include "pictureData.hpp"
#include "pictures.hpp"

class Convertor {
private:
  int quality = 75;
  const std::filesystem::path src;
  const std::filesystem::path dest;
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
  static int load_picture(const std::string &file_name, LibRaw &iProcessor) noexcept;

  /**
   * @brief Read data about loaded picture.
   *
   * @param iProcessor is reference for LibRaw processor
   * @return PictureData* pointer to loaded picture data
   */
  static PictureData *read_picture_data(LibRaw &iProcessor) noexcept;

  /**
   * @brief This method reset all buffer after processing all pictures. Does not
   * have to be called after every processed picture.
   */
  void reset_buffers() noexcept;

  /**
   * @brief Covert picture to jpg format.
   *
   * @param file_name is name of new file
   * @param procNum number of processor, convertor and buffer for this method
   */
  void convert_picture(const std::string &file_name, size_t procNum) noexcept;

  /**
   * @brief Compress and save bitmap to jpeg image.
   *
   * @param mem_image memory with bitmap image
   * @param procNum number of processor, convertor and buffer for this method
   * @param name is name of saved file
   */
  void save_jpg(const libraw_processed_image_t *mem_image, size_t procNum,
                const std::string &name) noexcept;

  /**
   * @brief Process picture and convert picture if convert is true.
   *
   * @param file_name is name of picture to procession
   * @param procNum number of processor, convertor and buffer for this method
   * @param pictures is shared pointer to class contains pictures
   * @param convert if it is true, picture will be converted
   */
  void process_picture(const std::string &file_name, size_t procNum,
                       std::shared_ptr<Pictures> pictures, bool convert = true) noexcept;

  /**
   * @brief Function that use processor, compressor and buffers based on procNum
   * and can run as thread.
   *
   * @param procNum is index of processor, compressor and buffers
   * @param index shared index for pictures must be atomic
   * @param pics collection of pictures names
   * @param picList collection of data about pictures
   */
  void process_list(size_t procNum, std::atomic<size_t> *index,
                    const std::vector<std::string> *pics,
                    std::shared_ptr<Pictures> picList) noexcept;

  /**
   * @brief Get the string with informations to print.
   *
   * @exception invalid_argument Throws if max is zero.
   *
   * @param last last printed item
   * @param act actual conversion position
   * @param max number of files to convertsion
   * @param pics collection od pictures names
   * @param verbose true if output should print converted filenames
   * @return std::string informations about conversion
   */
  static std::string get_info_string(size_t &last, size_t act, size_t max,
                                     const std::vector<std::string> &pics,
                                     bool verbose);

  /**
   * @brief Function to print informations about conversion.
   *
   * @param index point to number of converted file
   * @param pics collection of filenames
   * @param threadNum number of threads thats run
   * @param verbose true if output may contain filenames of converted files
   */
  static void print_info(const std::atomic<size_t> *index,
                         const std::vector<std::string> *pics, size_t threadNum,
                         bool verbose) noexcept;

public:
  bool verbose = false;
  bool quiet = false;

  /**
   * @brief Construct a new Convertor object.
   *
   * @param src_ path to source folder
   * @param dest_ path to destination folder
   * @param threads number of treads available for compressing images minimum is
   * 1, does not represents total number of threads
   */
  Convertor(const std::filesystem::path &src_, const std::filesystem::path &dest_,
            size_t threads = 1) noexcept;

  /**
   * @brief Destroy the Convertor object
   *
   */
  ~Convertor() noexcept;

  /**
   * @brief Set the quality of picture. Value must be between 1 and 100
   * included. If not, function throw std::invalid_argument exception.
   *
   * @exception invalid_argument Throws if quality is not in given range.
   *
   * @param quality_ is quality of compressed image.
   */
  void set_quality(unsigned int quality_);

  /**
   * @brief Convert all pictures in list.
   *
   * @param pics list of pictures to convert
   * @return std::shared_ptr<Pictures> pointer to class with list of loaded
   * data from pictures
   */
  std::shared_ptr<Pictures> conver_photos_list(const std::vector<std::string> &pics) noexcept;
};

#endif // CONVERTOR_H_INCLUDED
