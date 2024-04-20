#include "convertor.hpp"

#include <chrono>
#include <execution>
#include <ios>
#include <memory>
#include <string>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

using std::atomic, std::execution::par_unseq;
using std::cerr, std::endl, std::cout;
using std::for_each, std::to_string;
using std::shared_ptr, std::make_shared;
using std::string;
using std::thread;
using std::vector;

int Convertor::load_picture(const string &file_name, LibRaw &iProcessor) noexcept {
  int ret;
  // Return if LibRaw open file
  if ((ret = iProcessor.open_file(file_name.c_str())) != LIBRAW_SUCCESS) {
    cerr << "Error: " << libraw_strerror(ret) << endl;
    return ret;
  }
  return 0;
}

PictureData *Convertor::read_picture_data(LibRaw &iProcessor) noexcept {
  return new PictureData(
      iProcessor.imgdata.sizes.width, iProcessor.imgdata.sizes.height,
      iProcessor.imgdata.sizes.iwidth, iProcessor.imgdata.sizes.iheight,
      iProcessor.imgdata.sizes.top_margin, iProcessor.imgdata.sizes.left_margin,
      iProcessor.imgdata.sizes.raw_width, iProcessor.imgdata.sizes.raw_height);
}

void Convertor::reset_buffers() noexcept {
  // Free buffer allocated by TurboJPEG
  for_each(compImages.begin(), compImages.end(), [](auto &buffer) {
    tjFree(buffer);
    buffer = nullptr;
  });
  // Set all lengths to 0
  for_each(compImageSizes.begin(), compImageSizes.end(),
           [](auto &size) { size = 0; });
}

void Convertor::convert_picture(const string &file_name, size_t procNum) noexcept {

  LibRaw &proc = *iProcessors[procNum];
  proc.unpack();
  proc.imgdata.params.use_auto_wb = 1;
  proc.imgdata.params.bright = 2;

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
                         size_t procNum, const string &name) noexcept {

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
  f.write(reinterpret_cast<char *>(compressedImage), static_cast<std::streamsize>(*size));
  f.close();
}

void Convertor::process_picture(const string &file_name, size_t procNum,
                                shared_ptr<Pictures> pictures, bool convert) noexcept {
  // Get only name of file
  std::string name = file_name.substr(0, file_name.find_last_of('.'));

  // TODO check if image was loaded right
  load_picture(src / file_name, *iProcessors[procNum]);

  // Read picture data and add them to Pictures class
  // TODO use mutex for thread safe push backs
  // pictures->addPicture(name, read_picture_data(*iProcessors[procNum]));

  if (convert)
    convert_picture(dest / (name + ".jpg"), procNum);
}

void Convertor::process_list(size_t procNum, atomic<size_t> *index,
                             const vector<string> *pics,
                             shared_ptr<Pictures> picList) noexcept {
  size_t i = (*index)++;
  string print;
  size_t size = pics->size();

  while (size > i) {
    print = "thread: " + to_string(procNum) + "\tindex: ";
    print += to_string(i) + "\tname: " + (*pics)[i];
    process_picture((*pics)[i], procNum, picList);
    i = (*index)++;
  }
}

string Convertor::get_info_string(size_t &last, size_t act, size_t max,
                                  const vector<string> &pics, bool verbose) {
  string info = "\r";
  // TODO Enable bar with variable len
  size_t barLen = 20;
  if (max == 0) {
    throw std::invalid_argument("max must be positive number");
  }
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

  info += to_string(act) + "/" + to_string(max);
  return info;
}

void Convertor::print_info(const atomic<size_t> *index,
                           const vector<string> *pics, size_t threadNum,
                           bool verbose) noexcept {
  size_t max = pics->size();
  size_t last = 0;
  size_t i = 0;
  while (i < max) {
    cout << Convertor::get_info_string(last, i, max, *pics, verbose);
    cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    i = *index - threadNum;
  }
  cout << Convertor::get_info_string(last, i, max, *pics, verbose);
  cout << " DONE" << endl;
}

Convertor::Convertor(const fs::path &src_, const fs::path &dest_, size_t threads) noexcept
    : src(src_), dest(dest_), iProcessors(), tjCompressors() {

  // Create LibRaw and TurboJPEG instances
  for (size_t i = 0; i < threads; ++i) {
    iProcessors.push_back(new LibRaw());
    tjCompressors.push_back(tjInitCompress());
    compImages.push_back(nullptr);
    compImageSizes.push_back(0);
  }
}

Convertor::~Convertor() noexcept{
  // Destroy LibRaw and TurboJPEG instances
  for_each(par_unseq, iProcessors.begin(), iProcessors.end(),
           [](auto &proc) { delete proc; });
  for_each(par_unseq, tjCompressors.begin(), tjCompressors.end(),
           [](auto &comp) { tjDestroy(comp); });
}

void Convertor::set_quality(unsigned int quality_) {
  if (quality_ > 100 || quality_ == 0)
    throw std::invalid_argument("Quality must be between 1 and 100 included.");

  quality = static_cast<int>(quality_);
}

shared_ptr<Pictures> Convertor::conver_photos_list(const vector<string> &pics) noexcept {
  // Create shared pointer for Pictures class
  auto pictures = make_shared<Pictures>();

  // Create index for threads to read from vector
  atomic<size_t> index = 0;

  // Process all pictures
  vector<thread> threads;
  for (size_t i = 0; i < iProcessors.size(); ++i) {
    threads.push_back(
        thread(&Convertor::process_list, this, i, &index, &pics, pictures));
  }

  // Start thread to print info about converting
  thread tq;
  if (!quiet) {
    tq = thread(Convertor::print_info, &index, &pics, iProcessors.size(),
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
