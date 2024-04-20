#include "pictures.hpp"

#include <vector>

using std::string;
using std::vector;

void Pictures::addPicture(const string &name, std::unique_ptr<PictureData> pic) noexcept {
  pics.insert({name, std::move(pic)});
}

PictureData& Pictures::get_pic_data(const string &name) noexcept {
  return *pics[name];
}

vector<string> Pictures::listAll() noexcept {
  vector<string> ret;
  for (auto const &i : pics) {
    ret.push_back(i.first);
  }
  return ret;
}
