#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpp.h"



void free_pixel_arr_2bpp(Image_2bpp *Image)
{
	for (int i = 0; i < Image->height; i++) free(Image->pixel_arr[i]);
	free(Image->pixel_arr);
}



void get_color_table_2bpp(FILE *bmp_in, Image_2bpp *Image, DWORD biClrUsed)
{

}



void get_pixelarr_2bpp(FILE *bmp_in, Image_2bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth)
{

}



void flip_2bpp(Image_2bpp *Image)
{

}



void invert_2bpp(Image_2bpp *Image)
{

}



int rotate_2bpp(Image_2bpp *Image)
{

}



void do_instructions_2bpp(char *instructions, Image_2bpp *Image)
{

}



void write_2bpp(char *output_path, Image_2bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{

}
