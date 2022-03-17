#include "pictures.hpp"
#include <pstl/glue_execution_defs.h>
#include <vector>

using std::for_each;
using std::string;
using std::vector;
using std::execution::par_unseq;

Pictures::~Pictures() {
  for_each(par_unseq, pics.begin(), pics.end(),
           [](auto &pic) { delete pic.second; });
}

void Pictures::addPicture(const string &name, PictureData *pic) {
  pics.insert(make_pair(name, pic));
}

PictureData *Pictures::get_pic_data(const string &name) { return pics[name]; }

vector<string> Pictures::listAll() {
  vector<string> ret;
  for (auto const &i : pics) {
    ret.push_back(i.first);
  }
  return ret;
}