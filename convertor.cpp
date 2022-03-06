#include "convertor.h"

int Convertor::convert_photo(const std::string & file_name, LibRaw & iProcessor) {
    int ret;
    if ((ret = iProcessor.open_file(file_name.c_str())) != LIBRAW_SUCCESS) {
        std::cerr << "Error: " << libraw_strerror(ret) << std::endl;
        return ret;
    }

    iProcessor.unpack();

    PictureData *pic = new PictureData( iProcessor.imgdata.sizes.width,
                                        iProcessor.imgdata.sizes.height,
                                        iProcessor.imgdata.sizes.iwidth,
                                        iProcessor.imgdata.sizes.iheight,
                                        iProcessor.imgdata.sizes.top_margin,
                                        iProcessor.imgdata.sizes.left_margin,
                                        iProcessor.imgdata.sizes.raw_width,
                                        iProcessor.imgdata.sizes.raw_height);

    std::cout << *pic;

    iProcessor.dcraw_process();
    iProcessor.dcraw_ppm_tiff_writer("example.tiff");

    return 0;
}

std::shared_ptr<Pictures> Convertor::conver_photos_list(const std::vector<std::string> & pics, unsigned int threads = 1) {
    LibRaw iProcessor;
    auto pictures = std::make_shared<Pictures>();

    for (auto pic : pics) {
        convert_photo(pic, iProcessor);
    }

    return 0;
}