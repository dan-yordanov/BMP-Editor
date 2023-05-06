#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpp.h"



void free_pixel_arr_8bpp(Image_8bpp *Image)
{
	for (int i = 0; i < Image->height; i++) free(Image->pixel_arr[i]);
	free(Image->pixel_arr);
}



void get_color_table_8bpp(FILE *bmp_in, Image_8bpp *Image, DWORD biClrUsed)
{
	// seek to right after headers
	fseek(bmp_in, BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE, SEEK_SET);

	// calculating color count
	if (biClrUsed == 0)
	{
		// if biClrUsed is 0 that means the color count is the max possible for the bit count (2 ^ bpp)
		Image->color_count = 256; // 2 ^ 8 = 256
	}
	else
	{
		Image->color_count = biClrUsed;
	}

	// alocating memory for the color table
	Image->color_table = (RGBQUAD*) malloc(Image->color_count * RGBQUAD_SIZE);
	if (Image->color_table == NULL)
	{
		perror("Error allocatng memory for pixel array");
		fclose(bmp_in);
		exit(BPP_ERROR);
	}

	if (fread(Image->color_table, RGBQUAD_SIZE, Image->color_count, bmp_in) < Image->color_count)
	{
		perror("Error reading color table");
		fclose(bmp_in);
		free(Image->color_table);
		exit(BPP_ERROR);
	}
}



void get_pixelarr_8bpp(FILE *bmp_in, Image_8bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth)
{
	fseek(bmp_in, bfOffset, SEEK_SET);

	Image->height = abs(biHeight); // biHeight and biWidth can be negative meaning the rows/colums should be read in reverse
	Image->width = abs(biWidth);

	// each rows gets a padding such that the row byte count is divisible by 4 (padding = (4 - (width * bpp / 8) % 4) % 4))
	uint8_t padding_bytes = (4 - (biWidth * 8 / 8) % 4) % 4;

	// allocating memory for the pixel array and writing to it
	Image->pixel_arr = (Pixel_8bpp**) malloc(Image->height * sizeof(Pixel_8bpp*));
	if (Image->pixel_arr == NULL)
	{
		perror("Error allocatng memory for pixel array");
		fclose(bmp_in);
		free(Image->color_table);
		exit(BPP_ERROR);
	}

	for (int i = 0; i < Image->height; i++)
	{
		Image->pixel_arr[i] = (Pixel_8bpp*) malloc(Image->width * sizeof(Pixel_8bpp));
		if (Image->pixel_arr[i] == NULL)
		{
			perror("Error allocatng memory for pixel array row");
			fclose(bmp_in);
			free(Image->color_table);
			// freeing all previous rows
			for (int r = i - 1; r >= 0; r--) 
			{
				free(Image->pixel_arr[r]);
			}
			free(Image->pixel_arr);
			exit(BPP_ERROR);
		}
		
		// after allocating memory for the row we immediately write to it
		if (fread(Image->pixel_arr[i], sizeof(Pixel_8bpp), Image->width, bmp_in) < Image->width)
		{
			perror("Error writing pixel array row");
			fclose(bmp_in);
			free(Image->color_table);
			// freeing all rows up to current one
			for (int r = i; r >= 0; r--) 
			{
				free(Image->pixel_arr[r]);
			}
			free(Image->pixel_arr);
			exit(BPP_ERROR);
		}

		fseek(bmp_in, padding_bytes, SEEK_CUR); // skipping padding bytes
	}


	// we'll always set height and width in the output file header to be postive, to account for this we'll need to:
	// if width is negative flip the image, if height is negative reverse the row order

	// we already have a function to flip Image i.e. reverse column order if width is negative
	if (biWidth < 0)
	{
		flip_8bpp(Image);
	}

	// reversing row order if height is negative (rotating twice and flipping)
	if (biHeight < 0)
	{
		if (rotate_8bpp(Image) != 0 || rotate_8bpp(Image) != 0)
		{
			perror("Error rotating image to compensate for negative height");
			fclose(bmp_in);
			free(Image->pixel_arr);
			free_pixel_arr_8bpp(Image);
			exit(BPP_ERROR);
		}
		flip_8bpp(Image);
	}
}



void flip_8bpp(Image_8bpp *Image)
{
	for (int i = 0; i < Image->height; i++)
	{
		for  (int j = 0; j < Image->width / 2; j++)
		{
			// swapping elements that are at equal distance to the middle of the row
			Pixel_8bpp temp = Image->pixel_arr[i][j];
			Image->pixel_arr[i][j] = Image->pixel_arr[i][Image->width - 1 - j];
			Image->pixel_arr[i][Image->width - 1 - j] = temp;
		}
	}
}



void invert_8bpp(Image_8bpp *Image)
{
	for (int i = 0; i < Image->color_count; i++)
	{
		// inverting only the colors and not the reserved byte
		Image->color_table[i].red = ~Image->color_table[i].red;
		Image->color_table[i].green = ~Image->color_table[i].green;
		Image->color_table[i].blue = ~Image->color_table[i].blue;
	}
}



int rotate_8bpp(Image_8bpp *Image)
{
	WORD temp_height = Image->width;
	WORD temp_width = Image->height;

	// allocating memory for temp array
	Pixel_8bpp **temp_arr = (Pixel_8bpp**) malloc(temp_height * sizeof(Pixel_8bpp*));
	if (temp_arr == NULL)
	{
		return 1;
	}

	for (int i = 0; i < temp_height; i++)
	{
		temp_arr[i] = (Pixel_8bpp*) malloc(temp_width * sizeof(Pixel_8bpp));
		if (temp_arr[i] == NULL)
		{
			// freeing all previous rows
			for (int r = i - 1; r >= 0; r--) 
			{
				free(temp_arr[r]);
			}
			free(temp_arr);
			return 1;
		}
		// after allocating memory for the row we immediately write to it
		for (int j = 0; j < temp_width; j++) 
		{
			temp_arr[i][j] = Image->pixel_arr[j][Image->width - 1 - i];
		}
	}

	// freeing old pixel array
	free_pixel_arr_8bpp(Image);

	// swapping height and width of Image struct
	Image->height = temp_height;
	Image->width = temp_width;

	Image->pixel_arr = temp_arr;

	return 0;
}



void do_instructions_8bpp(char *instructions, Image_8bpp *Image)
{
	for (int i = 0; i < strlen(instructions); i++)
	{
		switch(instructions[i])
		{
			case 'a':
				flip_8bpp(Image);
				printf("\n%d. Изображението бе обърнато хоризонтално / Image was flipped\n", i + 1);
				break;
			case 'b':
				invert_8bpp(Image);
				printf("\n%d. Изобрежанието бе инвертирано / Image was inverted\n", i + 1);
				break;
			case 'c':
				if (rotate_8bpp(Image) != 0)
				{
					perror("Error rotating image");
					free(Image->color_table);
					free_pixel_arr_8bpp(Image);
					exit(BPP_ERROR);
				}
				printf("\n%d. Изобрежанието бе завъртяно надясно / Image was rotated to the right\n", i + 1);
				break;
			case 'd':
				printf("\n%d. Приключване на четене инструкции / Stopped reading instructions\n", i + 1);
				break;
			default:
				printf("\n%d. Невалиден избор / Invalid choice\n", i + 1);
				break;
		}
	}
}



void write_8bpp(char *output_path, Image_8bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{
	FILE *bmp_out = fopen(output_path, "wb");
	if (bmp_out == NULL)
	{
		perror("Failed to open output file");
		free(Image->color_table);
		free_pixel_arr_8bpp(Image);
		exit(BPP_ERROR);
	}


	// changing header and dheader info
	
	// the only thing that can change in header is bfSize and the things that can change in dheader are biWidth, biHeight, biSizeImage
	dheader->biHeight = Image->height;
	dheader->biWidth = Image->width;
	
	// each rows gets a padding such that the row byte count is divisible by 4 (padding = (4 - (width * bpp / 8) % 4) % 4)
	uint8_t padding_bytes = (4 - (Image->width * 8 / 8) % 4) % 4;
	dheader->biSizeImage = dheader->biHeight * (dheader->biWidth * sizeof(Pixel_8bpp) + padding_bytes); // includes only pixel array size
	
	// we don't have anything after the pixel array so all the metadata is stored before the bfOffset
	header->bfSize = dheader->biSizeImage + header->bfOffset; // includes size of metadata as well

	
	// not writing all at once to account for 2 bytes of padding after bfType(2 bytes)
	if (fwrite(header, 2, 1, bmp_out) < 1 || fwrite(&(header->bfSize), BITMAPFILEHEADER_SIZE - 2, 1, bmp_out) < 1)
	{
		perror("Error failed to write header");
		fclose(bmp_out);
		free(Image->color_table);
		free_pixel_arr_8bpp(Image);
		exit(BPP_ERROR);
	}


	// writing dheader
	if (fwrite(dheader, BITMAPINFOHEADER_SIZE, 1, bmp_out) < 1)
	{
		perror("Error failed to write dheader");
		fclose(bmp_out);
		free(Image->color_table);
		free_pixel_arr_8bpp(Image);
		exit(BPP_ERROR);
	}


	// writing color table and gap1
	if (fwrite(Image->color_table, RGBQUAD_SIZE, Image->color_count, bmp_out) < Image->color_count)
	{
		perror("Error failed to write color table to output file");
		fclose(bmp_out);
		free(Image->color_table);
		free_pixel_arr_8bpp(Image);
		exit(BPP_ERROR);
	}

	free(Image->color_table);


	// writing Gap1 between color table and pixel array if there is one
	// the differnce between the pixel array offset and the headers + color table size gives the size of Gap1 in bytes
	// NOTE: we don't have to worry about Gap2 which comes after the pixel array as we don't use ICC
	uint8_t Gap1_bytes = header->bfOffset - (BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE + RGBQUAD_SIZE * Image->color_count);
	if (fwrite(0, 1, Gap1_bytes, bmp_out) < Gap1_bytes)
	{
		perror("Error failed to write Gap1 to output file");
		fclose(bmp_out);
		free_pixel_arr_8bpp(Image);
		exit(BPP_ERROR);
	}


	// writing pixel array (last step as we don't have anything after it due to the paramters we're working with)
	char padding[padding_bytes];       // making the padding array
	memset(padding, 0, padding_bytes); // setting all the bytes in the padding to 0

	for (int i = 0; i < Image->height; i++)
	{
		// writing row
		for (int j = 0; j < Image->width; j++)
		{
			if (fwrite(&(Image->pixel_arr[i][j]), sizeof(Pixel_8bpp), 1, bmp_out) < 1)
			{
				perror("Error failed to write pixel array to output file");
				fclose(bmp_out);
				free_pixel_arr_8bpp(Image);
				exit(BPP_ERROR);
			}
		}
		// writing padding
		if (fwrite(padding, padding_bytes, 1, bmp_out) < 1 && padding_bytes != 0) // adding padding to the end of each row
		{
			perror("Error failed to write padding to output file");
			fclose(bmp_out);
			free_pixel_arr_8bpp(Image);
			exit(BPP_ERROR);
		}
	}

	free_pixel_arr_8bpp(Image);

	fclose(bmp_out);
}
