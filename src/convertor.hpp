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

  int load_picture(const std::string &, LibRaw &);

  PictureData *read_picture_data(LibRaw &);

  void convert_picture(const std::string &, LibRaw &);

  void process_picture(const std::string &, LibRaw &, std::shared_ptr<Pictures>,
                       bool);

public:
  bool verbose = false;
  bool quiet = false;

  Convertor(const fs::path &src, const fs::path &dest) : src(src), dest(dest) {}

  std::shared_ptr<Pictures> conver_photos_list(const std::vector<std::string> &,
                                               unsigned int);
};

#endif // CONVERTOR_H_INCLUDED