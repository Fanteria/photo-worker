#ifndef PICTURES_H_INCLUDED
#define PICTURES_H_INCLUDED

#include <map>
#include <string>

#include "pictureData.h"

class Pictures {
private:
    std::map<std::string , PictureData> pics;

public:
    Pictures() : pics() {}

    void addPicture(const std::string & name, const PictureData &);

    std::string listAll();
};

#endif // PICTURES_H_INCLUDED