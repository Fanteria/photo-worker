#include <filesystem>
#include <iostream>
#include <libraw/libraw.h>
#include <turbojpeg.h>

#include "picture.h"

class Convertor {
private:
    std::filesystem::path src;
    std::filesystem::path dest;

public:
    Convertor(const std::filesystem::path &src, const std::filesystem::path &dest) : 
        src(src), dest(dest) {}

    int convert_photo(const std::string &, LibRaw &);

};