#include <iostream>
#include <string>
#include <filesystem>
#include "libraw/libraw.h"

int process_image(char const *file);

int main() {

    std::string f = "./example.CR2";
    
    std::cout << std::filesystem::exists(f) << std::endl;
    process_image(f.c_str());

    delete file;

    return 0;
}

int process_image(char const *file) {
        // Let us create an image processor
        LibRaw iProcessor;

        // Open the file and read the metadata
        std::cout << iProcessor.open_file(file) << std::endl;

        // The metadata are accessible through data fields of the class
        printf("Image size: %d x %d\n",iProcessor.imgdata.sizes.width,iProcessor.imgdata.sizes.height);

        // Let us unpack the image
        iProcessor.unpack();

        // Convert from imgdata.rawdata to imgdata.image:
        iProcessor.raw2image();

        

        // Finally, let us free the image processor for work with the next image
        iProcessor.recycle();
        return 0;
}
