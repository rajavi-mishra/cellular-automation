/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	int width = (int) image->cols;
	int pix_pos = (width * row) + col;
	Color *orig = image->image[pix_pos];
	Color *pix_col = (struct Color*) malloc(sizeof(struct Color));
	if (pix_col) {
		pix_col->R = orig->R;
		pix_col->B = orig->B;
		pix_col->G = orig->G;
		return pix_col;
	} else {
		free(pix_col);
		exit(-1);
	}
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	struct Image *new_img = (struct Image*) malloc(sizeof(struct Image));
	new_img->rows = image->rows;
	new_img->cols = image->cols;
	int pixels = new_img->rows * new_img->cols;
	new_img->image = (struct Color**) malloc(pixels * sizeof(struct Color*));
	if (new_img->image) {
		for (int i = 0; i < new_img->rows; i++) {
			for (int k = 0; k < new_img->cols; k++) {
                		Color *pix_col = evaluateOnePixel(image, i, k);
				if ((pix_col->B)%2  == 0) {
					pix_col->R =  0;
					pix_col->G =  0;
					pix_col->B =  0;
				} else {
					pix_col->R = 255;
                                	pix_col->G = 255;
                                	pix_col->B = 255;
				}	
				int pix_pos = (new_img->cols * i) + k;
				new_img->image[pix_pos] = pix_col;
        		}
		} 	
		return new_img;
	} else {
		free(new_img->image);
                exit(-1);
        }
}

/*
Loads a .ppm from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{	
	if (argc == 2) {
		Image *img = readData(argv[1]);
		Image *to_print = steganography(img);
		writeData(to_print);
		freeImage(img);
		freeImage(to_print);
		exit(0);
		
	} else {
		exit(-1);
	}		
}
