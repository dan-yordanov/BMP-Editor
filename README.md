# BMP Editor

A simple bmp file editor that can flip, invert and rotate 1/2/4/8/16 bpp images.

## File structure
`src` folder:
- `bmp.c` - includes functions to store and print metadata of bmp files
- `io.c` - includes functions that get input bmp filepath, instruction set and output bmp filepath, either from cmd args or stdin input
- `1bpp.c`, `2bpp.c`, `4bpp.c`, `8bpp.c`, `16bpp.c` - each provides functonality for the respectful image's bit count like getting the color table and pixel array, executing the instructions in the instruction set and writing the output bmp file

`include` folder:
- `bmp.h` - header file of `bmp.c`
- `io.h` - header file of `io.c`
- `bpp.h` - header file of `1bpp.c`, `2bpp.c`, `4bpp.c`, `8bpp.c`, `16bpp.c`

`obj` folder - contains the .o files of the compiled .c files

`bin` folder - contains the executable file `bmp-editor.exe`

`samples` folder - contains sample bmp images with all used bit counts

`main.c` - calls functions to get the filepaths and the instruction set and after checking the bit count calls the respectful functions to do the instructions and write the ouput file

`build.bat` - batch script that builds the program

![project flowchart](https://github.com/dan-yordanov/BMP-editor/blob/main/Flowchart.png)


### Requirements for the bmp file
- Must use WINDOWS BITMAPINFOHEADER
- No compression is used i.e. compression type is BI_RGB
- Image needs to be 1/2/4/8/16 bpp (16 bpp images must use RGB 555 format as a consequence of the compression type)
- File extension needs to be `.bmp`


### How it works
- The program prompts the user to enter the filepath of the bmp image to be edited, if no such cmd argument is given. Afterwards the metadata of the given file is printed.
- Next, the user is prompted to enter instructions and a filepath for the output bmp file if no such cmd arguments are given.
- Afterwards the instructions are performed and the edited bmp is written to the given output filepath.
- Finally the metadata of the output bmp file is printed to the screen.


### How to use
The program takes up to 3 optional command line arguments:
1. Full filepath of the bmp file to be edited
2. Full filepath of the output edited bmp file
3. A string of characters resembling instructions
    - a - Flip the image around it's vertical axis
    - b - Invert the image i.e. invert the color of each pixel
    - c - Rotate the image 90 degrees to the right
    - d - Stop reading instructions

If no argument is given the user will be prompted to enter an input for the repectful variable.
Note: UTF-8 encoding should be enabled to display Cyrillic letters in the cmd console properly. In Windows cmd the command to do thats is "chcp 65001" and it is inclded in the `build.bat` batch file.


### References
- [BMP file format wiki](https://en.wikipedia.org/wiki/BMP_file_format)
- [WINDOWS BITMAPINFOHEADER documentation](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader)
- [BMP Suite - sample bmp images](http://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html)
- [Tutorialspots - 16 bpp sample bmp images and explanation of the structure](https://tutorialspots.com/bmp-file-structure-analysis-by-php-part-2-1092.html)
- [draw.io - used for generating the flowchart](https://www.drawio.com/)
