#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bpp.h"



void free_pixel_arr_1bpp(Image_1bpp *Image)
{
	for (int i = 0; i < Image->height; i++) free(Image->pixel_arr[i]);
	free(Image->pixel_arr);
}



void get_color_table_1bpp(FILE *bmp_in, Image_1bpp *Image, DWORD biClrUsed)
{
	// seek to right after headers
	fseek(bmp_in, BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE, SEEK_SET);

	// calculating color count
	if (biClrUsed == 0)
	{
		// if biClrUsed is 0 that means the color count is the max possible for the bit count (2 ^ bpp)
		Image->color_count = 2; // 2 ^ 1 = 2
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



void get_pixelarr_1bpp(FILE *bmp_in, Image_1bpp *Image, DWORD bfOffset, LONG biHeight, LONG biWidth)
{
	fseek(bmp_in, bfOffset, SEEK_SET);

	Image->height = abs(biHeight); // biHeight and biWidth can be negative meaning the rows/colums should be read in reverse
	Image->width = abs(biWidth);

	// calculating row size in bytes inlcuding padding, needs to be a multiple of 4 bytes i.e. 32 bits
	uint32_t row_size = ceil(biWidth / 32.0) * 4;


	// allocating memory for the pixel array and writing to it
	Image->pixel_arr = (Pixel_1bpp**) malloc(Image->height * sizeof(Pixel_1bpp*));
	if (Image->pixel_arr == NULL)
	{
		perror("Error allocatng memory for pixel array");
		fclose(bmp_in);
		free(Image->color_table);
		exit(BPP_ERROR);
	}

	BYTE buffer[row_size]; // we'll store each row to a buffer and store each bit to an array member

	// allocating memory for row and immediately writing to it
	for (int i = 0; i < Image->height; i++)
	{
		Image->pixel_arr[i] = (Pixel_1bpp*) malloc(Image->width * sizeof(Pixel_1bpp));
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
		if (fread(buffer, row_size, 1, bmp_in) < 1)
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

		// iterating over each byte and the each bit in it, breaking when last bit is reached
		int bit; // declaring it here so we can break outer loop
		for (int byte = 0; byte < row_size; byte++)
		{
			for (bit = 0; bit < 8; bit++)
			{
				// shifting and masking
				(Image->pixel_arr[i][byte * 8 + bit]).bit = (buffer[byte] >> (7 - bit)) & 1;
				if (byte * 8 + bit + 1 == biWidth) break;
			}
			if (byte * 8 + bit + 1 == biWidth) break;
		}

		fseek(bmp_in, bfOffset + row_size * (i + 1), SEEK_SET); // going to respectful row
	}


	// we'll always set height and width in the output file header to be postive, to account for this we'll need to:
	// if width is negative flip the image, if height is negative reverse the row order

	// we already have a function to flip Image i.e. reverse column order if width is negative
	if (biWidth < 0)
	{
		flip_1bpp(Image);
	}

	// reversing row order if height is negative (rotating twice and flipping)
	if (biHeight < 0)
	{
		if (rotate_1bpp(Image) != 0 || rotate_1bpp(Image) != 0)
		{
			perror("Error rotating image to compensate for negative height");
			fclose(bmp_in);
			free(Image->pixel_arr);
			free_pixel_arr_1bpp(Image);
			exit(BPP_ERROR);
		}
		flip_1bpp(Image);
	}
}



void flip_1bpp(Image_1bpp *Image)
{
	for (int i = 0; i < Image->height; i++)
	{
		for  (int j = 0; j < Image->width / 2; j++)
		{
			// swapping elements that are at equal distance to the middle of the row
			Pixel_1bpp temp = Image->pixel_arr[i][j];
			Image->pixel_arr[i][j] = Image->pixel_arr[i][Image->width - 1 - j];
			Image->pixel_arr[i][Image->width - 1 - j] = temp;
		}
	}
}



void invert_1bpp(Image_1bpp *Image)
{
	for (int i = 0; i < Image->color_count; i++)
	{
		// inverting only the colors and not the reserved byte
		Image->color_table[i].red = ~Image->color_table[i].red;
		Image->color_table[i].green = ~Image->color_table[i].green;
		Image->color_table[i].blue = ~Image->color_table[i].blue;
	}
}



int rotate_1bpp(Image_1bpp *Image)
{
	WORD temp_height = Image->width;
	WORD temp_width = Image->height;

	// allocating memory for temp array
	Pixel_1bpp **temp_arr = (Pixel_1bpp**) malloc(temp_height * sizeof(Pixel_1bpp*));
	if (temp_arr == NULL)
	{
		return 1;
	}

	for (int i = 0; i < temp_height; i++)
	{
		temp_arr[i] = (Pixel_1bpp*) malloc(temp_width * sizeof(Pixel_1bpp));
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
	free_pixel_arr_1bpp(Image);

	// swapping height and width of Image struct
	Image->height = temp_height;
	Image->width = temp_width;

	Image->pixel_arr = temp_arr;

	return 0;
}



void do_instructions_1bpp(char *instructions, Image_1bpp *Image)
{
	for (int i = 0; i < strlen(instructions); i++)
	{
		switch(instructions[i])
		{
			case 'a':
				flip_1bpp(Image);
				printf("\n%d. Изображението бе обърнато хоризонтално / Image was flipped\n", i + 1);
				break;
			case 'b':
				invert_1bpp(Image);
				printf("\n%d. Изобрежанието бе инвертирано / Image was inverted\n", i + 1);
				break;
			case 'c':
				if (rotate_1bpp(Image) != 0)
				{
					perror("Error rotating image");
					free(Image->color_table);
					free_pixel_arr_1bpp(Image);
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



void write_1bpp(char *output_path, Image_1bpp *Image, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{
	FILE *bmp_out = fopen(output_path, "wb");
	if (bmp_out == NULL)
	{
		perror("Failed to open output file");
		free(Image->color_table);
		free_pixel_arr_1bpp(Image);
		exit(BPP_ERROR);
	}

	// changing header and dheader info
	
	// the only thing that can change in header is bfSize and the things that can change in dheader are biWidth, biHeight, biSizeImage
	dheader->biHeight = Image->height;
	dheader->biWidth = Image->width;

	// each rows gets a padding such that the row byte count is divisible by 4 
	uint32_t row_size = ceil(dheader->biWidth / 32.0) * 4;
	dheader->biSizeImage = dheader->biHeight * row_size; // includes only pixel array size
	
	// we don't have anything after the pixel array so all the metadata is stored before the bfOffset
	header->bfSize = dheader->biSizeImage + header->bfOffset; // includes size of metadata as well

	// not writing all at once to account for 2 bytes of padding after bfType(2 bytes)
	if (fwrite(header, 2, 1, bmp_out) < 1 || fwrite(&(header->bfSize), BITMAPFILEHEADER_SIZE - 2, 1, bmp_out) < 1)
	{
		perror("Error failed to write header");
		fclose(bmp_out);
		free(Image->color_table);
		free_pixel_arr_1bpp(Image);
		exit(BPP_ERROR);
	}


	// writing dheader
	if (fwrite(dheader, BITMAPINFOHEADER_SIZE, 1, bmp_out) < 1)
	{
		perror("Error failed to write dheader");
		fclose(bmp_out);
		free(Image->color_table);
		free_pixel_arr_1bpp(Image);
		exit(BPP_ERROR);
	}


	// writing color table and gap1
	if (fwrite(Image->color_table, RGBQUAD_SIZE, Image->color_count, bmp_out) < Image->color_count)
	{
		perror("Error failed to write color table to output file");
		fclose(bmp_out);
		free(Image->color_table);
		free_pixel_arr_1bpp(Image);
		exit(BPP_ERROR);
	}

	free(Image->color_table);


	// writing Gap1 between color table and pixel array if there is one
	// the differnce between the pixel array offset and the headers + color table size gives the size of Gap1 in bytes
	// NOTE: we don't have to worry about Gap2 which comes after the pixel array as we don't use ICC
	int Gap1_bytes = header->bfOffset - (BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE + RGBQUAD_SIZE * Image->color_count);
	if (fwrite(0, 1, Gap1_bytes, bmp_out) < Gap1_bytes)
	{
		perror("Error failed to write Gap1 to output file");
		fclose(bmp_out);
		free_pixel_arr_1bpp(Image);
		exit(BPP_ERROR);
	}
	

	// writing pixel array (last step as we don't have anything after it due to the paramters we're working with)
	BYTE byte;
	BYTE bit_counter;

	// the amount of padding bytes that are completely empty i.e. all zeros
	BYTE empty_bytes = row_size - ceil(dheader->biWidth / 8.0);

	for (int i = 0; i < Image->height; i++)
	{
		// writing row byte by byte
		byte = 0;
		bit_counter = 0; // when it reaches 8 we start writing a new byte
		for (int j = 0; j < Image->width; j++)
		{
			byte |= Image->pixel_arr[i][j].bit << (7 - bit_counter);
			bit_counter++;

			if (bit_counter == 8)
			{
				// writing the byte and starting to "create" the next one
				if (fwrite(&byte, 1, 1, bmp_out) != 1)
				{
					perror("Error failed to write pixel array to output file");
					fclose(bmp_out);
					free_pixel_arr_1bpp(Image);
					exit(BPP_ERROR);
				}
				byte = 0;
				bit_counter = 0;
			}
		}

		// writing a byte that has less than 8 bits written to it if there's one
		if (bit_counter != 0)
		{
			if (fwrite(&byte, 1, 1, bmp_out) != 1)
			{
				perror("Error failed to write pixel array to output file");
				fclose(bmp_out);
				free_pixel_arr_1bpp(Image);
				exit(BPP_ERROR);
			}
		}
		
		// writing empty padding bytes (nothing gets written if empty bytes is 0)
		if (fwrite(0, 1, empty_bytes, bmp_out) != empty_bytes)
			{
				perror("Error failed to write padding to output file");
				fclose(bmp_out);
				free_pixel_arr_1bpp(Image);
				exit(BPP_ERROR);
			}
	}


	free_pixel_arr_1bpp(Image);

	fclose(bmp_out);
}
