#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"
#include "io.h"
#include "bpp.h"


int main(int argc, char *argv[])
{
	BITMAPFILEHEADER header;   // file header
	BITMAPINFOHEADER dheader;  // DIB header (working with WINDOWS BITMAPINFOHEADER)


	char input_path[MAX_FILEPATH_LEN + 1];
	char output_path[MAX_FILEPATH_LEN + 1];
	char instructions[MAX_INSTRUCTIONS_LEN + 1];


	// reads arg/stdin, stores it in input_path and opens the file at that location
	if (get_input_path(argc, argv, input_path) != 0)
	{
		exit(IO_ERROR);
	}
	
	// opening file at input path
	FILE *bmp_in = fopen(input_path, "rb");
	if (bmp_in == NULL)
	{
		perror("Error opening file");
		exit(BMP_ERROR);
	}

	// reads metadata from bmp file at input_path and then stores it in vars header and dheader
	get_meta(bmp_in, &header, &dheader);

	// prints metadata of input bmp which is stored in global vars header and dheader
	puts("\nInput file metadata: ");
	print_meta(&header, &dheader);

	// reads instructions and calls another function (do_instruction) to execute them
	if (get_instructions(argc, argv, instructions) != 0)
	{
		fclose(bmp_in);
		exit(IO_ERROR);
	}

	// reads arg/input and stores it in output_path
	if (get_output_path(argc, argv, output_path) != 0)
	{
		fclose(bmp_in);
		exit(IO_ERROR);
	}
	
	// reads pixel array from bmp file and stores it in the appropriate struct
	switch(dheader.biBitCount)
	{
		case 1:
		{
			Image_1bpp Image;
			
			get_pixelarr_1bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_1bpp(instructions, &Image);
			write_1bpp(output_path, &Image, &header, &dheader); // dynamic memory (pixel array, color table) gets freed in this funciton
			break;
		}
		case 2:
		{
			Image_2bpp Image;
			
			get_pixelarr_2bpp(bmp_in, &Image, header.bfOffset, dheader.biHeight, dheader.biWidth);
			fclose(bmp_in);
			do_instructions_2bpp(instructions, &Image);	
			write_2bpp(output_path, &Image, &header, &dheader);
			break;
		}
		case 4:
		{
			Image_4bpp Image;
			
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


	// prints metadata of output bmp which is stored in now modified vars header and dheader
	puts("\n\nOutput file metadata: ");
	print_meta(&header, &dheader);

	return 0;
}
