#include <filesystem>
#include <string>
#include <iostream>

class PictureData {
private:
    // Image size without frame
    const unsigned int width;
    const unsigned int height;

    // Output image size
    const unsigned int out_width;
    const unsigned int out_height;

    // Margin size for shift from left corner
    const unsigned int top_margin;
    const unsigned int left_margin;

    // Raw size with frame
    const unsigned int raw_width;
    const unsigned int raw_height;

public:
    PictureData(const unsigned int width,
            const unsigned int height,
            const unsigned int out_width,
            const unsigned int out_height,
            const unsigned int top_margin,
            const unsigned int left_margin,
            const unsigned int raw_width,
            const unsigned int raw_height) : 
            width(width), height(height),
            out_width(out_width), out_height(out_height),
            top_margin(top_margin), left_margin(left_margin),
            raw_width(raw_width), raw_height(raw_height) {}

    std::pair<unsigned int, unsigned int> get_image_size();

    std::pair<unsigned int, unsigned int> get_output_image_size();

    std::pair<unsigned int, unsigned int> get_margin_size();

    std::pair<unsigned int, unsigned int> get_raw_image_size();

    friend std::ostream& operator<< (std::ostream &, const PictureData &);
};