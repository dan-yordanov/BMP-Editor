#ifndef BPP_H
#define BPP_H

#include "bmp.h"

#define RGBQUAD_SIZE 4
#define BPP_ERROR 3 // used for an exit() code in the bpp .c files


// Pixel and Image structs for the different bpp values


// RGBQUAD structure that's used in color tables (bpp <= 8 and uses little-endian)
typedef struct
{
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE reserved; // should be zero
} RGBQUAD;



// 1 bpp (uses color table)

typedef struct
{
	BYTE bit: 1; // using struct requires at least 1 byte per pixel but we'll only care about the least significant bit
} Pixel_1bpp;

typedef struct
{
	DWORD height; 
	DWORD width;
	Pixel_1bpp **pixel_arr;        // 2D array of pixels
	RGBQUAD *color_table;          // 1D array of RGBQUAD values in the color table
	DWORD color_count;             // count of colors in color table
} Image_1bpp;



// 2 bpp (uses color table)

typedef struct
{
	BYTE bits: 2; // using struct requires at least 1 byte per pixel but we'll only care about the 2 least significant bits
} Pixel_2bpp;

typedef struct
{
	DWORD height; 
	DWORD width;
	Pixel_2bpp **pixel_arr;        // 2D array of pixels
	RGBQUAD *color_table;          // 1D array of RGBQUAD values in the color table
	DWORD color_count;             // count of colors in color table
} Image_2bpp;



// 4 bpp (uses color table)

typedef struct
{
	BYTE bits: 4; // using struct requires at least 1 byte per pixel but we'll only care about the 4 least significant bits
} Pixel_4bpp;

typedef struct
{
	DWORD height; 
	DWORD width;
	Pixel_4bpp **pixel_arr;        // 2D array of pixels
	RGBQUAD *color_table;          // 1D array of RGBQUAD values in the color table
	DWORD color_count;             // count of colors in color table
} Image_4bpp;



// 8 bpp (uses color table)

typedef struct
{
	BYTE byte; // 1 byte/8 bits per pixel
} Pixel_8bpp;

typedef struct
{
	DWORD height; 
	DWORD width;
	Pixel_8bpp **pixel_arr;        // 2D array of pixels
	RGBQUAD *color_table;          // 1D array of RGBQUAD values in the color table
	DWORD color_count;             // count of colors in color table
} Image_8bpp;



// 16 bpp (using RGB 555 format since compression is BI_RGB and sorting it as BGR since bmp uses little-endian byte ordering)

typedef struct
{
	WORD blue : 5;                 // 5 bits for red component
	WORD green : 5;                // 5 bits for green component
	WORD red : 5;                  // 5 bits for blue component (+1 unused bit)
	WORD unused_bit: 1;            // 1 bit unused (should be 0)
} Pixel_16bpp;

typedef struct 
{
	DWORD height; 
	DWORD width;
	Pixel_16bpp **pixel_arr;       // 2D array of pixels
} Image_16bpp;



// functions for different bpp values

// 1bpp

//------------------------------------------------------------------------------------------
// DESCRIPTION: Gets color table stored in bmp_in and stores it to the color table member
// of the Image struct and also determines the color count which is another member of Image
//
// PARAMETERS: bmp_in - input bmp file, Image - struct that contains data about the
// bmp image including a dynamic array that stores the color table, biClrUsed - member of
// the dheader struct that stores the color count of the table
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void get_color_table_1bpp(FILE *bmp_in, Image_1bpp *Image, DWORD biClrUsed);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Gets the pixel array stored in bmp_in and stores it to the pixel array
// member of the Image struct, also reads height and width of image from header, and stores
// it to Image struct members height and width
//
// PARAMETERS: bmp_in - input bmp file, Image - struct that contains data about the
// bmp image including a 2d dynamic array that stores the pixel array, bfOffset - header
// member that shows where the pixel array begins, biHeight - dheader member that stores the
// height of the image, biWidth - dheader member that stores the width of the image
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void get_pixelarr_1bpp(FILE *bmp_in, Image_1bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Frees the allocated memory of the 2d dynamic pixel array in the Image struct
//
// PARAMETERS: Image - struct that contains data about the bmp image including a 2d dynamic 
// array that stores the pixel array
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void free_pixel_arr_1bpp(Image_1bpp *Image);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Iterates over each instruction in the instruction set and calls the
// respectful function (flip, invert, rotate) which modifies the pixel array stored in Image
//
// PARAMETERS: instructons - array where the inputted instructions are stored, Image -
// struct that contains data about the bmp image including a 2d dynamic array that stores
// the pixel array
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void do_instructions_1bpp(char *instructions, Image_1bpp *Image);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Flips the image by flipping the 2d array stored in the pixel array member
// of the Image struct(n*k matrix) around its vertical axis
//
// PARAMETERS: Image - struct that contains data about the bmp image including a 2d dynamic 
// array that stores the pixel array
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void flip_1bpp(Image_1bpp *Image);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Inverts the image by inverting each color stored in the color table
//
// PARAMETERS: Image - struct that contains data about the bmp image including a dynamic
// array that stores the color table
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void invert_1bpp(Image_1bpp *Image);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Rotates the image by 90 degrees to the right, by rotating the 2d array
// stored in the pixel array member of the Image struct(n*k matrix)
//
// PARAMETERS: Image - struct that contains data about the bmp image including a 2d dynamic 
// array that stores the pixel array 
//
// RETURN VALUE: 0 on success, -1 on failure (NOTE: unlike other instructions this one
// returns an exit code as it can fail at multiple points) 
//------------------------------------------------------------------------------------------
int rotate_1bpp(Image_1bpp *Image);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Writes the output file to the inputted output filepath using needed data:
// new metadata stored in headers, new color table and pixel array stored in Image struct
//
// PARAMETERS: output_path - string that stores the inputted output filepath for the edited
// bmp file, Image - struct that contains data about the bmp image including a 2d dynamic 
// array that stores the pixel array and a 1d array that stores the color table, header - 
// file header, dheader - Device Independent Bitmap(DIB) header, which should be WINDOWS 
// BITMAPINFOHEADER
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void write_1bpp(char *output_path, Image_1bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// NOTE: No documentation for the functions below as they essentially have the same functionality as the functions above.
// The only difference is in the invert function for the 16 bpp image which inverts the pixel values in the pixel array
// themselves, instead of inverting values in a color table as there isn't one. (no get_color_table function as well)


// 2bpp
void get_color_table_2bpp(FILE *bmp_in, Image_2bpp *Image, DWORD biClrUsed);
void get_pixelarr_2bpp(FILE *bmp_in, Image_2bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth);
void free_pixel_arr_2bpp(Image_2bpp *Image);

void do_instructions_2bpp(char *instructions, Image_2bpp *Image);
void flip_2bpp(Image_2bpp *Image);
void invert_2bpp(Image_2bpp *Image);
int rotate_2bpp(Image_2bpp *Image);

void write_2bpp(char *output_path, Image_2bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 4bpp
void get_color_table_4bpp(FILE *bmp_in, Image_4bpp *Image, DWORD biClrUsed);
void get_pixelarr_4bpp(FILE *bmp_in, Image_4bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth);
void free_pixel_arr_4bpp(Image_4bpp *Image);

void do_instructions_4bpp(char *instructions, Image_4bpp *Image);
void flip_4bpp(Image_4bpp *Image);
void invert_4bpp(Image_4bpp *Image);
int rotate_4bpp(Image_4bpp *Image);

void write_4bpp(char *output_path, Image_4bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 8bpp
void get_color_table_8bpp(FILE *bmp_in, Image_8bpp *Image, DWORD biClrUsed);
void get_pixelarr_8bpp(FILE *bmp_in, Image_8bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth);
void free_pixel_arr_8bpp(Image_8bpp *Image);

void do_instructions_8bpp(char *instructions, Image_8bpp *Image);
void flip_8bpp(Image_8bpp *Image); 
void invert_8bpp(Image_8bpp *Image);
int rotate_8bpp(Image_8bpp *Image);

void write_8bpp(char *output_path, Image_8bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 16bpp
void get_pixelarr_16bpp(FILE *bmp_in, Image_16bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth);
void free_pixel_arr_16bpp(Image_16bpp *Image);

void do_instructions_16bpp(char *instructions, Image_16bpp *Image);
void flip_16bpp(Image_16bpp *Image);
void invert_16bpp(Image_16bpp *Image);
int rotate_16bpp(Image_16bpp *Image);

void write_16bpp(char *output_path, Image_16bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);

#endif
