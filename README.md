# Photo worker

This project implements the conversion from `.CR2` raw format (_potentially more raw files based on LibRaw functionality, but it is untested_) from Cannon camera to `.jpg` images. Used are two external libraries [LibRaw](https://www.libraw.org) for converting raw images to memory bitmaps and [TurboJPEG](https://libjpeg-turbo.org) for fast compress images to `jpg`.

You can use this project also for synchronizing converted and raw photos if you will delete some of them or rename all photos in your directories.

## Usage

The program has only CLI. The first argument should be a type of work. Options are:

- `convert` - Convert photos from source folder to destination folder.
- `rename` - Rename files from source and/or destination folder
- `sync` - From the source folder delete files that are missing in the dest file.

The program has several flags. Not all flags are used by all arguments, this represents the table below. But if you use them, the program will ignore them.

|    name     | shortcut | default | convert  |  rename  |   sync   |
| :---------: | :------: | :-----: | :------: | :------: | :------: |
|   source    |    s     |    .    | &#x2713; | &#x2713; | &#x2713; |
| destination |    d     |    .    | &#x2713; | &#x2713; | &#x2713; |
|   threads   |    t     |    1    | &#x2713; | &#x2717; | &#x2717; |
|    quiet    |    q     |  false  | &#x2713; | &#x2713; | &#x2713; |
|   verbose   |    v     |  false  | &#x2713; | &#x2713; | &#x2713; |
|     ask     |    a     |  false  | &#x2717; | &#x2713; | &#x2713; |
|    name     |    n     |   ""    | &#x2717; | &#x2713; | &#x2717; |

As you can see, you can use flag quiet and verbose at the same time. If you do it, flag quiet will be stronger and mute all output. Also, if you change the source, but not the destination, the destination will be `source/jpg`. You can change both to the same path to avoid this.

## Documentation

You can check GitHub pages for documentation. If you want something else, the code is documented in headers files with Doxygen syntax. So you can read it int header files or use [Doxygen](https://www.doxygen.nl/index.html) to generate it.

## Conclusion

Be aware of using this code without backing up your files. I'm trying my best, but the code is not tested enough and can behave in unexpected ways. If you don't know, how the program works, try it on testing photos. Also feels free to contact me or create an issue if you find a bug or strange behavior of the program. And in the end, I'm sorry for my crappy English.
