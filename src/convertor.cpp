#include "convertor.hpp"
#include "pictures.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <libraw/libraw_const.h>
#include <memory>
#include <ostream>
#include <pstl/glue_execution_defs.h>
#include <string>
#include <thread>
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
  // pictures->addPicture(name, read_picture_data(*iProcessors[procNum]));

  if (convert)
    convert_picture(dest / (name + ".jpg"), procNum);
}

void Convertor::process_list(size_t procNum, std::atomic<size_t> *index,
                             const std::vector<std::string> *pics,
                             std::shared_ptr<Pictures> picList) {
  size_t i = (*index)++;
  std::string print;
  size_t size = pics->size();

  while (size > i) {
    print = "thread: " + std::to_string(procNum) + "\tindex: ";
    print += std::to_string(i) + "\tname: " + (*pics)[i];
    // std::cout << print + "\tstart\n";
    //  TODO in multithred processing causes segfault
    process_picture((*pics)[i], procNum, picList);
    // std::cout << print + "\tdone\n";
    i = (*index)++;
  }
}

std::string Convertor::get_info_string(size_t &last, size_t act, size_t max,
                                       const std::vector<std::string> &pics,
                                       bool verbose) {
  std::string info = "\r";
  // TODO Enable bar with variable len
  size_t barLen = 20;
  size_t actLen = act * barLen / max;

  if (verbose) {
    for (; last < act; ++last)
      // TODO solve carrige return some better way
      info += pics[last] + " converted             \n";
  }

  info += "[";
  for (size_t i = 0; i < barLen; ++i)
    info += (i > actLen) ? "." : "#";
  info += "]";

  info += std::to_string(act) + "/" + std::to_string(max);
  return info;
}

void Convertor::print_info(std::atomic<size_t> *index,
                           const std::vector<std::string> *pics,
                           size_t threadNum, bool verbose) {
  size_t max = pics->size();
  size_t last = 0;
  size_t i = 0;
  while (i < max) {
    std::cout << Convertor::get_info_string(last, i, max, *pics, verbose);
    std::cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    i = *index - threadNum;
  }
  std::cout << Convertor::get_info_string(last, i, max, *pics, verbose);
  std::cout << " DONE" << std::endl;
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
  std::for_each(std::execution::par_unseq, iProcessors.begin(),
                iProcessors.end(), [](auto &proc) { delete proc; });
  std::for_each(std::execution::par_unseq, tjCompressors.begin(),
                tjCompressors.end(), [](auto &comp) { tjDestroy(comp); });
}

void Convertor::set_quality(unsigned int quality) {
  if (quality > 100 || quality == 0)
    throw std::invalid_argument("Quality must be between 1 and 100 included.");

  this->quality = quality;
}

std::shared_ptr<Pictures>
Convertor::conver_photos_list(const std::vector<std::string> &pics) {
  // Create shared pointer for Pictures class
  auto pictures = std::make_shared<Pictures>();

  // Create index for threads to read from vector
  std::atomic<size_t> index = 0;

  // Process all pictures
  std::vector<std::thread> threads;
  for (size_t i = 0; i < iProcessors.size(); ++i) {
    threads.push_back(std::thread(&Convertor::process_list, this, i, &index,
                                  &pics, pictures));
  }

  // Start thread to print info about converting
  std::thread tq;
  if (!quiet) {
    tq = std::thread(Convertor::print_info, &index, &pics, iProcessors.size(),
                     verbose);
  }

  // Wait for end of converting
  for (auto &t : threads)
    t.join();

  // Wait for end of printing
  if (!quiet) {
    tq.join();
  }

  // Free buffers memory
  reset_buffers();
  return pictures;
}