/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"


//Helper for wrap index 
int wrap(int row, int col, int max_row, int max_col) {
        if (row == -1) {
                row = max_row - 1;
        } if (row == max_row) {
                row = 0;
        } if (col == -1) {
                col = max_col - 1;
        } if (col == max_col) {
                col = 0;
        }
        int pix_pos = (row * max_col) + col;
        return pix_pos;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *new_color = malloc(sizeof(struct Color));
	if (new_color) {
        	uint8_t new_red = 0;
        	uint8_t new_green = 0;
        	uint8_t new_blue = 0;
        	int width = image->cols;
		int height = image-> rows;
        	Color *curr_color = image->image[wrap(row, col, height, width)];	 		
	
        	Color **neighbors[8] = {&(image->image[wrap(row - 1, col, height, width)]), &(image->image[wrap(row + 1, col, height, width)]), &(image->image[wrap(row, col - 1, height, width)]), &(image->image[wrap(row, col + 1, height, width)]), &(image->image[wrap(row - 1, col - 1, height, width)]), &(image->image[wrap(row - 1, col + 1, height, width)]), &(image->image[wrap(row + 1, col - 1, height, width)]), &(image->image[wrap(row + 1, col + 1, height, width)])};


        	uint8_t red = curr_color->R;
        	for (int k = 0; k < 8; k++) {
                	int curr_bit = (red >> k) & 1;
                	int alive = 0;
                	for (int i = 0; i < 8; i++) {
			
                        	int n_bit = ((*neighbors[i])->R >> k) & 1;
                        	if (n_bit == 1) {
                                	alive += 1;
                        	}
                	}
			 	
                	if (curr_bit == 1) {
                        	if (((rule >> (alive + 9)) & 1) == 1) {
                                	new_red = new_red | (1 << k);
                        	}	
			 
                	} else {
                        	if (((rule >> alive) & 1) == 1) {
                                	new_red = new_red | (1 << k);
                        	} 
                	}
				
        	}	
        
        	uint8_t green = curr_color->G;
        	for (int k = 0; k < 8; k++) {
                	int curr_bit = (green >> k) & 1;
                	int alive = 0;
                	for (int i = 0; i < 8; i++) {
				
                        	int n_bit = ((*neighbors[i])->G >> k) & 1;
                        	if (n_bit == 1) {
                                	alive += 1;
                        	}	
                	}	
                	if (curr_bit == 1) {
                        	if (((rule >> (alive + 9)) & 1) == 1) {
                                	new_green = new_green | (1 << k);
                        	}
                	} else {
                        	if (((rule >> alive) & 1) == 1) {
                                	new_green = new_green | (1 << k);
                        	}	       
                	}	
        	}	      

        	uint8_t blue = curr_color->B;
        	for (int k = 0; k < 8; k++) {
                	int curr_bit = (blue >> k) & 1;
                	int alive = 0;
			for (int i = 0; i < 8; i++) {
				
                        	int n_bit = ((*neighbors[i])->B >> k) & 1;
                        	if (n_bit == 1) {
                                	alive += 1;
                        	}
                	}	      
                	if (curr_bit == 1) {
                        	if (((rule >> (alive + 9)) & 1) == 1) {
                                	new_blue = new_blue | (1 << k);
                        	}	
                	} else {
                        	if (((rule >> alive) & 1) == 1) {
                                	new_blue = new_blue | (1 << k);
                        	}	       
                	}
        	}

        	new_color->R = new_red;
        	new_color->G = new_green;
        	new_color->B = new_blue;
        	return new_color;
	} else {
		free(new_color);
		exit(-1);
	}
}
	

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	struct Image *new_img = (struct Image*) malloc(sizeof(struct Image));
        if (new_img) {
		new_img->rows = image->rows;
        	new_img->cols = image->cols;
        	int pixels = new_img->rows * new_img->cols;
        	new_img->image = (struct Color**) malloc(pixels * sizeof(struct Color*));
        	if (new_img->image) {
                	for (int i = 0; i < new_img->rows; i++) {
                        	for (int k = 0; k < new_img->cols; k++) {
                                	Color *pix_col = evaluateOneCell(image, i, k, rule);
                                	int pix_pos = (new_img->cols * i) + k;
                               		new_img->image[pix_pos] = pix_col;
                        	}
                	}
                	return new_img;
        	} else {
                	free(new_img->image);
                	exit(-1);
        	}	
	} else {
		free(new_img);
		exit(-1);
	}
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc == 3) {
		uint32_t rule = strtol(argv[2], 0, 16);
		Image *img = readData(argv[1]);
                Image *to_print = life(img, rule);
                writeData(to_print);
                freeImage(img);
                freeImage(to_print);
                exit(0);
	} else {
		exit(-1);
	}
}

