#ifndef BPP_H
#define BPP_H

#include "bmp.h"

#define BPP_ERROR 3


// Pixel and PixelArr structs for different bpp values

// 1 bpp (uses color table)

typedef struct
{
	BYTE bit: 1; // using struct requires at least 1 byte per pixel but we'll only care about the least significant bit
} Pixel_1bpp;

typedef struct
{
	DWORD height;                   // 4 bytes 
	DWORD width;                    // 4 bytes
	Pixel_1bpp **pixel_arr;         // 2D array of pixels
} Image_1bpp;



// 2 bpp (uses color table)

typedef struct
{
	BYTE bits: 2; // using struct requires at least 1 byte per pixel but we'll only care about the 2 least significant bits
} Pixel_2bpp;

typedef struct
{
	DWORD height;                   // 4 bytes 
	DWORD width;                    // 4 bytes
	Pixel_2bpp **pixel_arr;         // 2D array of pixels
} Image_2bpp;



// 4 bpp (uses color table)

typedef struct
{
	BYTE bits: 4; // using struct requires at least 1 byte per pixel but we'll only care about the 4 least significant bits
} Pixel_4bpp;

typedef struct
{
	DWORD height;                   // 4 bytes 
	DWORD width;                    // 4 bytes
	Pixel_4bpp **pixel_arr;         // 2D array of pixels
} Image_4bpp;



// 8 bpp (uses color table)

typedef struct
{
	BYTE byte; // 1 byte
} Pixel_8bpp;

typedef struct
{
	DWORD height;                    // 4 bytes 
	DWORD width;                     // 4 bytes
	Pixel_8bpp **pixel_arr;          // 2D array of pixels
} Image_8bpp;



// 16 bpp (using RGB 555 format since compression is BI_RGB and sorting it as BGR as bmp uses little-endian byte ordering)

typedef struct
{
	WORD blue : 5;       // 5 bits for red component
	WORD green : 5;      // 5 bits for green component
	WORD red : 5;        // 5 bits for blue component (+1 unused bit)
	WORD unused_bit: 1;  // 1 bit unused
} Pixel_16bpp;

typedef struct 
{
	DWORD height;                    // 4 bytes 
	DWORD width;                     // 4 bytes
	Pixel_16bpp **pixel_arr;         // 2D array of pixels
} Image_16bpp;



// functions for different bpp values

// 1bpp
void get_pixelarr_1bpp(FILE *bmp_in, char *input_path, Image_1bpp *Image, DWORD biOffset, LONG biHeight, LONG biWidth);
void do_instructions_1bpp(FILE *bmp_in, char * instructions, Image_1bpp *Image);

void flip_1bpp(Image_1bpp *Image);
void invert_1bpp(Image_1bpp *Image);
int rotate_1bpp(Image_1bpp *Image);


void free_pixel_arr_1bpp(Image_1bpp *Image);
void write_1bpp(FILE *bmp_in, char *output_path, Image_1bpp *Image,  BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 2bpp
void get_pixelarr_2bpp(FILE *bmp_in, char *input_path, Image_2bpp *Image, DWORD biOffset, LONG biHeight, LONG biWidth);
void do_instructions_2bpp(FILE *bmp_in, char * instructions, Image_2bpp *Image);

void flip_2bpp(Image_2bpp *Image);
void invert_2bpp(Image_2bpp *Image);
int rotate_2bpp(Image_2bpp *Image);

void free_pixel_arr_2bpp(Image_2bpp *Image);
void write_2bpp(FILE *bmp_in, char *output_path, Image_2bpp *Image,  BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 4bpp
void get_pixelarr_4bpp(FILE *bmp_in, char *input_path, Image_4bpp *Image, DWORD biOffset, LONG biHeight, LONG biWidth);
void do_instructions_4bpp(FILE *bmp_in, char * instructions, Image_4bpp *Image);

void flip_4bpp(Image_4bpp *Image);
void invert_4bpp(Image_4bpp *Image);
int rotate_4bpp(Image_4bpp *Image);

void free_pixel_arr_4bpp(Image_4bpp *Image);
void write_4bpp(FILE *bmp_in, char *output_path, Image_4bpp *Image,  BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 8bpp
void get_pixelarr_8bpp(FILE *bmp_in, char *input_path, Image_8bpp *Image, DWORD biOffset, LONG biHeight, LONG biWidth);
void do_instructions_8bpp(FILE *bmp_in, char * instructions, Image_8bpp *Image);

void flip_8bpp(Image_8bpp *Image); 
void invert_8bpp(Image_8bpp *Image);
int rotate_8bpp(Image_8bpp *Image);

void free_pixel_arr_8bpp(Image_8bpp *Image);
void write_8bpp(FILE *bmp_in, char *output_path, Image_8bpp *Image,  BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);


// 16bpp
void get_pixelarr_16bpp(FILE *bmp_in, char *input_path, Image_16bpp *Image, DWORD biOffset, LONG biHeight, LONG biWidth);
void do_instructions_16bpp(FILE *bmp_in, char * instructions, Image_16bpp *Image);

void flip_16bpp(Image_16bpp *Image);
void invert_16bpp(Image_16bpp *Image);
int rotate_16bpp(Image_16bpp *Image);

void free_pixel_arr_16bpp(Image_16bpp *Image);
void write_16bpp(FILE *bmp_in, char *output_path, Image_16bpp *Image,  BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);

#endif
