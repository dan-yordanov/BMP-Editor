#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdint.h>

#define BITMAPFILEHEADER_SIZE 14
#define BITMAPINFOHEADER_SIZE 40
#define BMP_ERROR 2        // used for an exit() code in bmp.c

// some Windows data types

typedef uint8_t  BYTE;     // 1 byte
typedef uint16_t WORD;     // 2 bytes 
typedef uint32_t DWORD;    // 4 bytes
typedef int32_t  LONG;     // 4 bytes (signed)

// Bitmap file header
// size of struct = 16 bytes (14 + 2 padding)

typedef struct 
{
	WORD  bfType;          // 2 bytes + 2 bytes padding (must be "BM")
	DWORD bfSize;          // 4 bytes
	WORD  bfReserved1;     // 2 bytes (should be 0)
	WORD  bfReserved2;     // 2 bytes (should be 0)
	DWORD bfOffset;	       // 4 bytes
} BITMAPFILEHEADER;

// Window BITMAPINFOHEADER - the DIB header we're working with
// size of struct = 40 bytes (no padding)

typedef struct 
{
	DWORD biSize;          // 4 bytes (must be 40)
	LONG  biWidth;         // 4 bytes (signed)
	LONG  biHeight;        // 4 bytes (signed)
	WORD  biPlanes;        // 2 bytes (must be 1)
	WORD  biBitCount;      // 2 bytes (must be 1, 2, 4, 8 or 16)
	DWORD biCompression;   // 4 bytes (must be 0 - BI_RGB)
	DWORD biSizeImage;     // 4 bytes
	LONG  biXPelsPerMeter; // 4 bytes (signed)
	LONG  biYPelsPerMeter; // 4 bytes (signed)
	DWORD biClrUsed;       // 4 bytes
	DWORD biClrImportant;  // 4 bytes
} BITMAPINFOHEADER;


//------------------------------------------------------------------------------------------
// DESCRIPTION: Reads metadata from bmp_in and stores it to struct vars header and dheader
//
// PARAMETERS: bmp_in - input bmp file, header - file header, dheader - Device Independent
// Bitmap(DIB) header, which should be WINDOWS BITMAPINFOHEADER
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void get_meta(FILE *bmp_in, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Prints formatted and readable strings that contain metadata stored in header
// and dheader
//
// PARAMETERS: header - file header, dheader - Device Independent Bitmap(DIB) header, which
// should be WINDOWS BITMAPINFOHEADER
//
// RETURN VALUE: void
//------------------------------------------------------------------------------------------
void print_meta(BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader);

#endif
