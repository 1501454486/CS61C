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
	//YOUR CODE HERE
	if( !image ) {
		return NULL;
	}
	// ensure that the image is valid;
	Color *color = (Color *)malloc( sizeof( Color ) );
	if( image->image[row][col].B & 1 ) {
		// if the last digit of B is 1, then the color should be white;
		color->R = color->G = color->B = 255;
	} else {
		color->R = color->G = color->B = 0;
	}
	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	if( !image ) {
		return NULL;
	}
	Image *newImage = (Image *)malloc( sizeof( Image ) );
	newImage->cols = image->cols;
	newImage->rows = image->rows;
	newImage->image = (Color **)malloc( image->rows * sizeof( Color *) );
	for( int i = 0; i < image->rows; i++ ) {
		newImage->image[i] = (Color *)malloc( image->cols * sizeof( Color ) );
		for( int j = 0; j < image->cols; j++ ) {
			Color *evaluatePixel = evaluateOnePixel( image, image->rows, image->cols );
			newImage->image[i][j] = *evaluatePixel;
			free( evaluatePixel );
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	char* filename = argv[1];

	Image *image = readData( filename );
	if( !image ) {
		free( filename );
		return -1;
	}
	Image *newImage = steganography( image );
	if( !newImage ) {
		free( filename );
		free( image );
		return -1;
	}
	writeData( newImage );

	free( newImage );
	free( image );
	free( filename );

	return 0;
}