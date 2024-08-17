/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen( filename, "r" );
	if( !fp ) {
		return NULL;
	}

	Image *imageNode = (Image*)malloc(sizeof(Image));
	char format[8];
	int maxColorVal;
	uint32_t R, G, B;
	
	fscanf(fp, "%s", format);
	fscanf(fp, "%u %u", &imageNode->cols, &imageNode->rows );
	fscanf(fp, "%d", &maxColorVal );
	
	imageNode->image = (Color**)malloc(imageNode->rows * sizeof(Color*));
	for( int i = 0; i < imageNode->rows; i++ ) {
		imageNode->image[i] = (Color*)malloc( imageNode->cols * sizeof(Color) );
		for( int j = 0; j < imageNode->cols; j++ ) {
			fscanf(fp, "%u %u %u", &R, &G, &B );
			imageNode->image[i][j].R = (uint8_t)R;
			imageNode->image[i][j].G = (uint8_t)G;
			imageNode->image[i][j].B = (uint8_t)B;
		}
	}
	fclose(fp);
	return imageNode;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	if( image ) {
		// if image is valid
		printf("P3\n");
		printf("%d %d\n255\n", image->cols, image->rows );
		for( int i = 0; i < image->rows; i++ ) {
			for( int j = 0; j < image->cols - 1; j++ ) {
				printf("%3u %3u %3u   ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B );
			}
			printf("%3u %3u %3u\n", image->image[i][image->cols-1].R, image->image[i][image->cols-1].G, image->image[i][image->cols-1].B );
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if( image ) {
		for( int i = 0; i < image->rows; i++ ) {
			if( image->image[i] ) free( image->image[i] );
		}
		free( image );
	}
}