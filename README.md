# Photo worker

This project implements conversion from `.CR2` raw format (_potentially more raw files based on LibRaw functionality, but it is untested_) from Cannon camera to `.jpg` images. Used are two external libraries [LibRaw](https://www.libraw.org) for convert raw images to memory bitmaps and [TurboJPEG](https://libjpeg-turbo.org) for fast compress images to `jpg`.

You can use this project also for synchronize converted and raw photos if you will delete some of them or rename all photos in yours directories.

## Usage

Program have only CLI. First argument should be type of work. Options are:

- `convert` - Convert photos from source folder to destination folder.
- `rename` - Rename files from source and/or destination folder
- `sync` - From source folder delete files thats missing in dest file.

Program have several flags. Not all flags is used by all arguments, this represents table below. But if you use them, program will ignore them.

|    name     | shortcut | default | convert  |  rename  |   sync   |
| :---------: | :------: | :-----: | :------: | :------: | :------: |
|   source    |    s     |    .    | &#x2713; | &#x2713; | &#x2713; |
| destination |    d     |    .    | &#x2713; | &#x2713; | &#x2713; |
|   threads   |    t     |    1    | &#x2713; | &#x2717; | &#x2717; |
|    quiet    |    q     |  false  | &#x2713; | &#x2713; | &#x2713; |
|   verbose   |    v     |  false  | &#x2713; | &#x2713; | &#x2713; |
|     ask     |    a     |  false  | &#x2717; | &#x2713; | &#x2713; |
|    name     |    n     |   ""    | &#x2717; | &#x2713; | &#x2717; |

As you can see, you can use flag quiet and verbose at same time. If you do it, flag quiet will be stronger and mute all output. Also, if you change source, but not destination, destination will be `source/jpg`. You can change both to same path to avoid this.

## Documentation

You can check github pages for documentation. If you want something else, code is documented in headers files with Doxygen syntax. So you can read it int header files or use [Doxygen](https://www.doxygen.nl/index.html) to generate it.

## Conclusion

Be aware of use it this code without back up yours file. I'm trying my best, but code is not tested enough and can behave in unexpected ways. If you don't know how program is works, try it on testing photos. Also feels free to contact me or create issue if you find bug or strange behavior of the program. And in the end, I'm sorry for my crappy english.
