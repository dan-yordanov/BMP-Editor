#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpp.h"



void free_pixel_arr_1bpp(Image_1bpp *Image)
{
	for (int i = 0; i < Image->height; i++) free(Image->pixel_arr[i]);
	free(Image->pixel_arr);
}



void get_color_table_1bpp(FILE *bmp_in, Image_1bpp *Image, DWORD biClrUsed)
{

}



void get_pixelarr_1bpp(FILE *bmp_in, Image_1bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth)
{

}



void flip_1bpp(Image_1bpp *Image)
{

}



void invert_1bpp(Image_1bpp *Image)
{

}



int rotate_1bpp(Image_1bpp *Image)
{

}



void do_instructions_1bpp(char *instructions, Image_1bpp *Image)
{

}



void write_1bpp(char *output_path, Image_1bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{

}
