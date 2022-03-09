#ifndef PICTURES_H_INCLUDED
#define PICTURES_H_INCLUDED

#include <map>
#include <string>

#include "pictureData.hpp"

class Pictures {
private:
  std::map<std::string, PictureData *> pics;

public:
  Pictures() : pics() {}
  ~Pictures();

  void addPicture(const std::string &name, PictureData *);

  PictureData *get_pic_data(const std::string &);

  void get_picture(const std::string &);

  std::string listAll();
};

#endif // PICTURES_H_INCLUDED