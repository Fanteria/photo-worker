#include "convertor.hpp"
#include <algorithm>
#include <cstddef>
#include <libraw/libraw_const.h>
#include <turbojpeg.h>

int Convertor::load_picture(const std::string &file_name, LibRaw &iProcessor) {
  int ret;
  // Return if LibRaw open file
  if ((ret = iProcessor.open_file(file_name.c_str())) != LIBRAW_SUCCESS) {
    std::cerr << "Error: " << libraw_strerror(ret) << std::endl;
    return ret;
  }
  return 0;
}

PictureData *Convertor::read_picture_data(LibRaw &iProcessor) {
  return new PictureData(
      iProcessor.imgdata.sizes.width, iProcessor.imgdata.sizes.height,
      iProcessor.imgdata.sizes.iwidth, iProcessor.imgdata.sizes.iheight,
      iProcessor.imgdata.sizes.top_margin, iProcessor.imgdata.sizes.left_margin,
      iProcessor.imgdata.sizes.raw_width, iProcessor.imgdata.sizes.raw_height);
}

void Convertor::reset_buffers() {
  // Free buffer allocated by TurboJPEG
  std::for_each(compImages.begin(), compImages.end(), [](auto &buffer) {
    tjFree(buffer);
    buffer = nullptr;
  });
  // Set all lengths to 0
  std::for_each(compImageSizes.begin(), compImageSizes.end(),
                [](auto &size) { size = 0; });
}

void Convertor::convert_picture(const std::string &file_name, size_t procNum) {

  LibRaw &proc = *iProcessors[procNum];
  proc.unpack();

  // Process picture to byte array
  proc.dcraw_process();

  // Make memory buffer
  int errCode;
  libraw_processed_image_t *mem_image = proc.dcraw_make_mem_image(&errCode);
  // TODO Check errCode

  // TODO Create implementation for thumbnails.
  if (mem_image->type == LIBRAW_IMAGE_BITMAP) {
    save_jpg(mem_image, procNum, file_name);
  }

  // Free memory buffer
  LibRaw::dcraw_clear_mem(mem_image);
}

void Convertor::save_jpg(const libraw_processed_image_t *mem_image,
                         size_t procNum, const std::string &name) {

  unsigned long int *size = &compImageSizes[procNum];
  unsigned char *compressedImage = compImages[procNum];

  // Check if comressed buffer is long enough and free memory if is allocated.
  // Allocate new is not necessary, tjCompress will allocate if size is 0.
  if (*size < mem_image->data_size) {
    if (compressedImage != nullptr)
      tjFree(compressedImage);
    *size = 0;
  }

  // Compress and save image if it is bitmap
  tjCompress2(tjCompressors[procNum], mem_image->data, mem_image->width, 0,
              mem_image->height, TJPF_RGB, &compressedImage, size, TJSAMP_444,
              quality, TJFLAG_FASTDCT);

  // Save compressed image to binary file
  auto f = std::fstream(name, std::ios::out | std::ios::binary);
  f.write((char *)compressedImage, *size);
  f.close();
}

void Convertor::process_picture(const std::string &file_name, size_t procNum,
                                std::shared_ptr<Pictures> pictures,
                                bool convert) {
  // Get only name of file
  std::string name = file_name.substr(0, file_name.find_last_of('.'));

  // TODO check if image was loaded right
  load_picture(src / file_name, *iProcessors[procNum]);

  // Read picture data and add them to Pictures class
  pictures->addPicture(name, read_picture_data(*iProcessors[procNum]));

  if (convert)
    convert_picture(dest / (name + ".jpg"), 0);
}

Convertor::Convertor(const fs::path &src, const fs::path &dest, size_t threads)
    : src(src), dest(dest), iProcessors(), tjCompressors() {

  // Create LibRaw and TurboJPEG instances
  for (size_t i = 0; i < threads; ++i) {
    iProcessors.push_back(new LibRaw());
    tjCompressors.push_back(tjInitCompress());
    compImages.push_back(nullptr);
    compImageSizes.push_back(0);
  }
}

Convertor::~Convertor() {
  // Destroy LibRaw and TurboJPEG instances
  std::for_each(iProcessors.begin(), iProcessors.end(),
                [](auto &proc) { delete proc; });
  std::for_each(tjCompressors.begin(), tjCompressors.end(),
                [](auto &comp) { tjDestroy(comp); });
}

void Convertor::set_quality(unsigned int quality) {
  if (quality > 100 || quality == 0)
    throw std::invalid_argument("Quality must be between 1 and 100 included.");

  this->quality = quality;
}

std::shared_ptr<Pictures>
Convertor::conver_photos_list(const std::vector<std::string> &pics,
                              unsigned int threads) {
  LibRaw iProcessor;

  // Create shared pointer for Pictures class
  auto pictures = std::make_shared<Pictures>();

  // Process all pictures
  for (auto const &pic : pics) {
    process_picture(pic, 0, pictures);
  }
  reset_buffers();
  return pictures;
}