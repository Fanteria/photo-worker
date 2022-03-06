#include "pictures.h"

void Pictures::addPicture(const std::string & name, PictureData * pic) {
    pics.insert(make_pair(name, pic));
}

std::string Pictures::listAll() {
    std::string ret;
    for(auto const & i : pics) {
        ret += i.first;
    }
    return ret;
}