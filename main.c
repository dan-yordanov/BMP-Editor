//------------------------------------------------------------------------------------------
//
// Project name: BMP Editor
//
// Author: Danail Yordanov
// 
// Date of creation: April 2023
//
//------------------------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"
#include "io.h"
#include "bpp.h"


int main(int argc, char *argv[])
{
	BITMAPFILEHEADER header;   // file header
	BITMAPINFOHEADER dheader;  // DIB header (should be WINDOWS BITMAPINFOHEADER)


	char input_path[MAX_FILEPATH_LEN + 1];        // filepath of input bmp
	char instructions[MAX_INSTRUCTIONS_LEN + 1];  // instruction set
	char output_path[MAX_FILEPATH_LEN + 1];       // filepath of output bmp


	// read cmd arg/stdin for input bmp filepath and store it in input_path
	if (get_input_path(argc, argv, input_path) != 0)
	{
		exit(IO_ERROR);
	}
	
	// open file at input_path
	FILE *bmp_in = fopen(input_path, "rb");
	if (bmp_in == NULL)
	{
		perror("Error opening file");
		exit(BMP_ERROR);
	}

	// read metadata from bmp file at input_path and then store it in vars header and dheader
	get_meta(bmp_in, &header, &dheader);

	// print metadata of input bmp stored in vars header and dheader
	puts("\nInput file metadata: ");
	print_meta(&header, &dheader);

	// read cmd arg/stdin for the instruction set and store it in instructions
	if (get_instructions(argc, argv, instructions) != 0)
	{
		fclose(bmp_in);
		exit(IO_ERROR);
	}

	// read cmd arg/stdin for ouput bmp filepath and store it in output_path
	if (get_output_path(argc, argv, output_path) != 0)
	{
		fclose(bmp_in);
		exit(IO_ERROR);
	}
	
	// check the image's bit count and call the respectful functions for the bpp
	// get the color table(except for 16bpp) and pixel array, and then execute the given instructions
	// finally write the ouput bmp file from the new data (modified metadata, color table and pixel array)
	switch(dheader.biBitCount)
	{
		case 1:
		{
			Image_1bpp Image;
			get_color_table_1bpp(bmp_in, &Image, dheader.biClrUsed);
			get_pixelarr_1bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_1bpp(instructions, &Image);
			write_1bpp(output_path, &Image, &header, &dheader); // dynamic memory (pixel array, color table) gets freed in this funciton
			break;
		}
		case 2:
		{
			Image_2bpp Image;
			get_color_table_2bpp(bmp_in, &Image, dheader.biClrUsed);
			get_pixelarr_2bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_2bpp(instructions, &Image);	
			write_2bpp(output_path, &Image, &header, &dheader);
			break;
		}
		case 4:
		{
			Image_4bpp Image;
			get_color_table_4bpp(bmp_in, &Image, dheader.biClrUsed);
			get_pixelarr_4bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_4bpp(instructions, &Image);	
			write_4bpp(output_path, &Image, &header, &dheader);
			break;
		}
		case 8:
		{
			Image_8bpp Image;
			get_color_table_8bpp(bmp_in, &Image, dheader.biClrUsed);
			get_pixelarr_8bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_8bpp(instructions, &Image);	
			write_8bpp(output_path, &Image, &header, &dheader);
			break;
		}
		case 16:
		{
			Image_16bpp Image;
			get_pixelarr_16bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_16bpp(instructions, &Image);	
			write_16bpp(output_path, &Image, &header, &dheader);
			break;
		}	

	}


	// print metadata of output bmp which is stored in now modified vars header and dheader
	puts("\n\nOutput file metadata: ");
	print_meta(&header, &dheader);

	return 0;
}
