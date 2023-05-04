#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpp.h"



void free_pixel_arr_4bpp(Image_4bpp *Image)
{
	for (int i = 0; i < Image->height; i++) free(Image->pixel_arr[i]);
	free(Image->pixel_arr);
}



void get_color_table_4bpp(FILE *bmp_in, Image_4bpp *Image, DWORD biClrUsed)
{

}



void get_pixelarr_4bpp(FILE *bmp_in, Image_4bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth)
{

}



void flip_4bpp(Image_4bpp *Image)
{

}



void invert_4bpp(Image_4bpp *Image)
{

}



int rotate_4bpp(Image_4bpp *Image)
{

}



void do_instructions_4bpp(char *instructions, Image_4bpp *Image)
{

}



void write_4bpp(char *output_path, Image_4bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{

}
