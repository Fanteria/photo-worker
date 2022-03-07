#include "pictures.hpp"

Pictures::~Pictures() {
    for(auto & pic : pics) {
        delete pic.second;
    }
}

void Pictures::addPicture(const std::string & name, PictureData * pic) {
    pics.insert(make_pair(name, pic));
}

std::string Pictures::listAll() {
    std::string ret;
    for(auto const & i : pics) {
        ret += i.first + "\n";
    }
    return ret;
}