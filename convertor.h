#ifndef CONVERTOR_H_INCLUDED
#define CONVERTOR_H_INCLUDED

#include <filesystem>
#include <iostream>
#include <libraw/libraw.h>
#include <turbojpeg.h>

#include "pictures.h"
#include "pictureData.h"

class Convertor {
private:
    std::filesystem::path src;
    std::filesystem::path dest;

public:
    Convertor(const std::filesystem::path &src, const std::filesystem::path &dest) : 
        src(src), dest(dest) {}

    int convert_photo(const std::string &, LibRaw &);

    std::shared_ptr<Pictures> conver_photos_list(const std::vector<std::string> &, unsigned int);
};

#endif //CONVERTOR_H_INCLUDED