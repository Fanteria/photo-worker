#include "convertor.hpp"

int Convertor::load_picture(const std::string & file_name, LibRaw & iProcessor) {
    int ret;
    if ((ret = iProcessor.open_file(file_name.c_str())) != LIBRAW_SUCCESS) {
        std::cerr << "Error: " << libraw_strerror(ret) << std::endl;
        return ret;
    }
    return 0;
}

PictureData * Convertor::read_picture_data(LibRaw & iProcessor) {
    return new PictureData( iProcessor.imgdata.sizes.width,
                            iProcessor.imgdata.sizes.height,
                            iProcessor.imgdata.sizes.iwidth,
                            iProcessor.imgdata.sizes.iheight,
                            iProcessor.imgdata.sizes.top_margin,
                            iProcessor.imgdata.sizes.left_margin,
                            iProcessor.imgdata.sizes.raw_width,
                            iProcessor.imgdata.sizes.raw_height);
}

void Convertor::convert_picture(const std::string & file_name, LibRaw & iProcessor) {
    std::cout << file_name << std::endl;
    iProcessor.unpack();
    iProcessor.dcraw_process();
    iProcessor.dcraw_ppm_tiff_writer(file_name.c_str());
}

void Convertor::process_picture(const std::string & file_name, LibRaw & iProcessor, std::shared_ptr<Pictures> pictures, bool convert = true) {
    std::string name = file_name.substr(0, file_name.find_last_of('.'));
    load_picture(src / file_name, iProcessor);
    pictures->addPicture(name, read_picture_data(iProcessor));
    if(convert)
        convert_picture(dest / (name + ".tiff"), iProcessor);
}

std::shared_ptr<Pictures> Convertor::conver_photos_list(const std::vector<std::string> & pics, unsigned int threads = 1) {
    LibRaw iProcessor;
    auto pictures = std::make_shared<Pictures>();

    for (auto const & pic : pics) {
        process_picture(pic, iProcessor, pictures);
    }

    iProcessor.recycle();
    return pictures;
}