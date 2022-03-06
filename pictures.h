#include <map>
#include <string>

#include "picture.h"

class Pictures {
private:
    std::map<std::string , PictureData> pics;

public:
    Pictures() : pics() {}

    void addPicture(const std::string & name, const PictureData &);
};