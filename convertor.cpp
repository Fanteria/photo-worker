#include "convertor.h"

int Convertor::convert_photo(const std::string & file, LibRaw & iProcessor) {
    int ret;
    if ((ret = iProcessor.open_file(file.c_str())) != LIBRAW_SUCCESS) {
        std::cerr << "Error: " << libraw_strerror(ret) << std::endl;
        return ret;
    }

    iProcessor.unpack();

    Picture *pic = new Picture(file,
                            iProcessor.imgdata.sizes.width,
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