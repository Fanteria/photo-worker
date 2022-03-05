#include <filesystem>
#include <string>

class Picture {
    std::filesystem::path raw;
    std::filesystem::path converted;
    std::string name;
    unsigned int width;
    unsigned int height;

    Picture(const std::string & name) : name(name) {}
};