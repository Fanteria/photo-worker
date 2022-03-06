#include "pictures.h"

void Pictures::addPicture(const std::string & name, const PictureData & pic) {
    pics.insert(make_pair(name, pic));
}