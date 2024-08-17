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

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	int aliveNeighbors = 0;
	// the size of the image;
	int height = image->rows;
	int width = image->cols;

	// calculate alive neighbor cells around the given cell;
	int dx[] = {-1 ,0, 1, -1, 1, -1, 0, 1};
	int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	for( int i = 0; i < 8; i++ ) {
		int x = ( col + dx[i] ) % width;
		int y = ( row + dy[i] ) % height;
		// if one of RGB is none-zero, then the cell is alive, vice versa;
		if( image->image[x][y].R || image->image[x][y].G || image->image[x][y].B ) {
			aliveNeighbors++;
		}
	}

	// updating the status of the given cell:
	Color *color = (Color *)malloc( sizeof( Color ) );
	if( !color ) {
		return NULL;
	}

	uint32_t mask = 1 << aliveNeighbors;
	if( image->image[row][col].R || image->image[row][col].G || image->image[row][col].B ) {
		// for the living cell:
		if( rule >> 9 & mask ) {
			// the cell still lives;
			color->R = color->G = color->B = 255;
		} else {
			// the cell dies;
			color->R = color->G = color->B = 0;
		}	
	} else {
		// for dead cells:
		if( rule & mask ) {
			color->R = color->G = color->B = 255;
		} else {
			color->R = color->G = color->B = 0;
		}
	}

	return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *newImage = (Image *)malloc( sizeof( Image ) );
	if( !newImage ) {
		return NULL;
	}

	newImage->cols = image->cols;
	newImage->rows = image->rows;
	newImage->image = (Color **)malloc( newImage->rows * sizeof( Color * ) );
	// if allocation fails, free the mamories that have been allocated;
	if( !newImage->image ) {
		free( newImage );
		return NULL;
	}
	
	for( int i = 0; i < newImage->rows; i++ ) {
		newImage->image[i] = ( Color * )malloc( newImage->cols * sizeof( Color ) );
		// if allocation fails, free the mamories that have been allocated;
		if( !newImage->image[i] ) {
			freeImage( newImage );
			return NULL;
		}
		for( int j = 0; j < newImage->cols; j++ ) {
			Color *color = evaluateOneCell( image, i, j, rule );
			newImage->image[i][j] = *color;
		}
	}

	return newImage;
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
	//YOUR CODE HERE
	char *filename = argv[1];

	Image *image = readData( filename );
	if( !image ) {
		// if read fails
		return -1;
	}

	uint32_t rule = strtol( argv[2], NULL, 16 );
	Image *newImage = life( image, rule );
	freeImage( image );

	writeData( newImage );
	freeImage( newImage );
	return 0;
}