# BMP Editor

A simple bmp file editor that can flip, invert and rotate 1/2/4/8/16 bpp images. 

### Requirements for the bmp file
- Must use WINDOWS BITMAPINFOHEADER
- No compression is used i.e. compression type is BI_RGB
- Image needs to be 1/2/4/8/16 bpp (16 bpp images must use RGB 555 format due to compression type)
- File extension needs to be .bmp

### How it works
- The program prompts the user to enter the filepath of the bmp image to be edited, if no such cmd argument is given. Afterwards the metadata of the given file is printed.
- Next, the user is prompted to enter instructions and a filepath for the output bmp file if no such cmd arguments are given.
- Afterwards the instructions are performed and the edited bmp is stored to the given output filepath.
- Finally the metadata of the output bmp file is printed to the screen.


### How to use
The program takes up to 3 optional command line arguments:
1. Full filepath of the bmp file to be edited
2. Full filepath of the output edited bmp file
3. A string of characters resembling instructions
    - a - Flip the image around vertical axis
    - b - Invert the image
    - c - Rotate the image 90 degrees to the right
    - d - Stop reading instructions

If no argument is given the user will be prompted to enter an input.

### References
- [BMP file format wiki](https://en.wikipedia.org/wiki/BMP_file_format)
- [WINDOWS BITMAPINFOHEADER documentation](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader)
- [BMP Suite - sample bmp images](http://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html)
- [Tutorialspots - 16 bpp sample bmp images and explanation of the structure](https://tutorialspots.com/bmp-file-structure-analysis-by-php-part-2-1092.html)
