#include "pictures.hpp"

Pictures::~Pictures() {
    std::for_each( pics.begin(), pics.end(), [](auto & pic) {
        delete pic.second;
    });
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