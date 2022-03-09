#include "pictures.hpp"

Pictures::~Pictures() {
  std::for_each(pics.begin(), pics.end(), [](auto &pic) { delete pic.second; });
}

void Pictures::addPicture(const std::string &name, PictureData *pic) {
  pics.insert(make_pair(name, pic));
}

PictureData *Pictures::get_pic_data(const std::string &name) {
  return pics[name];
}

std::string Pictures::listAll() {
  std::string ret;
  for (auto const &i : pics) {
    ret += i.first + "\n";
  }
  return ret;
}