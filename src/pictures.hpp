#ifndef PICTURES_H_INCLUDED
#define PICTURES_H_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "pictureData.hpp"

class Pictures {
private:
  std::map<std::string, PictureData *> pics;

public:
  /**
   * @brief Construct a new Pictures object
   *
   */
  Pictures() : pics() {}

  /**
   * @brief Destroy the Pictures object
   *
   */
  ~Pictures();

  /**
   * @brief
   *
   * @param name is name of picture
   * @param pic pointer to picture data class, that will be freed in destructor
   */
  void addPicture(const std::string &name, PictureData *pic);

  /**
   * @brief Get the picture data object
   *
   * @param name is name of picture
   * @return PictureData* pointer to class with picture data
   */
  PictureData *get_pic_data(const std::string &name);

  /**
   * @brief Get list od all picture names
   *
   * @return std::vector<std::string> is list of all picture names
   */
  std::vector<std::string> listAll();
};

#endif // PICTURES_H_INCLUDED