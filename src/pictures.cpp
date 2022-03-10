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

std::vector<std::string> Pictures::listAll() {
  std::vector<std::string> ret;
  for (auto const &i : pics) {
    ret.push_back(i.first);
  }
  return ret;
}