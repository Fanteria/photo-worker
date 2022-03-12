#include "convertor.hpp"
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

void Convertor::convert_picture(const std::string &file_name,
                                LibRaw &iProcessor, tjhandle *compressor,
                                unsigned char *compressedImage,
                                long unsigned int *size, int quality) {

  iProcessor.unpack();

  // Process picture to byte array
  iProcessor.dcraw_process();

  // Make memory buffer
  int errCode;
  libraw_processed_image_t *mem_image =
      iProcessor.dcraw_make_mem_image(&errCode);
  // TODO Check errCode

  // TODO Create implementation for thumbnails.
  if (mem_image->type == LIBRAW_IMAGE_BITMAP) {
    save_jpg(mem_image, compressor, compressedImage, size, file_name, quality);
  }

  // Free memory buffer
  LibRaw::dcraw_clear_mem(mem_image);
}

void Convertor::save_jpg(const libraw_processed_image_t *mem_image,
                         tjhandle *compressor, unsigned char *compressedImage,
                         long unsigned int *size, const std::string &name,
                         int quality) {

  // Check if comressed buffer is long enough and free memory if is allocated.
  // Allocate new is not necessary, tjCompress will allocate if size is 0.
  if (*size < mem_image->data_size) {
    if (compressedImage != nullptr)
      tjFree(compressedImage);
    *size = 0;
  }

  // Compress and save image if it is bitmap
  tjCompress2(*compressor, mem_image->data, mem_image->width, 0,
              mem_image->height, TJPF_RGB, &compressedImage, size, TJSAMP_444,
              quality, TJFLAG_FASTDCT);

  // Save compressed image to binary file
  auto f = std::fstream(name, std::ios::out | std::ios::binary);
  f.write((char *)compressedImage, *size);
  f.close();
}

void Convertor::process_picture(const std::string &file_name,
                                LibRaw &iProcessor,
                                std::shared_ptr<Pictures> pictures,
                                bool convert) {
  // Get only name of file
  std::string name = file_name.substr(0, file_name.find_last_of('.'));

  load_picture(src / file_name, iProcessor);

  // Read picture data and add them to Pictures class
  pictures->addPicture(name, read_picture_data(iProcessor));
  if (convert) {
    tjhandle compressor = tjInitCompress();
    long unsigned int size = 0;
    unsigned char *compressedImage = nullptr;

    convert_picture(dest / (name + ".jpg"), iProcessor, &compressor,
                    compressedImage, &size, 75);

    tjDestroy(compressor);
    tjFree(compressedImage);
  }
}

std::shared_ptr<Pictures>
Convertor::conver_photos_list(const std::vector<std::string> &pics,
                              unsigned int threads) {
  LibRaw iProcessor;

  // Create shared pointer for Pictures class
  auto pictures = std::make_shared<Pictures>();

  // Process all pictures
  for (auto const &pic : pics) {
    process_picture(pic, iProcessor, pictures);
  }

  // Recycle data from LibRaw iProcessor
  iProcessor.recycle();
  return pictures;
}