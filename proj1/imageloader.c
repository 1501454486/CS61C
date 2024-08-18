#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

// Opens a .ppm P3 image file, and constructs an Image object. 
Image *readData(char *filename) 
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        return NULL;
    }

    Image *imageNode = (Image *)malloc(sizeof(Image));
    if (!imageNode) {
        perror("Error allocating memory");
        fclose(fp);
        return NULL;
    }

    char format[8];
    int maxColorVal;
    uint32_t R, G, B;

    fscanf(fp, "%s", format);
    if (strcmp(format, "P3")) {
        fprintf(stderr, "Invalid file format: %s\n", format);
        free(imageNode);
        fclose(fp);
        return NULL;
    }

    fscanf(fp, "%u %u", &imageNode->cols, &imageNode->rows);
    fscanf(fp, "%d", &maxColorVal);
    printf("Loaded Image: %u cols, %u rows\n", imageNode->cols, imageNode->rows);

    imageNode->image = (Color **)malloc(imageNode->rows * sizeof(Color *));
    if (!imageNode->image) {
        perror("Error allocating memory for rows");
        free(imageNode);
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < imageNode->rows; i++) {
        imageNode->image[i] = (Color *)malloc(imageNode->cols * sizeof(Color));
        if (!imageNode->image[i]) {
            perror("Error allocating memory for cols");
            for (int j = 0; j < i; j++) {
                free(imageNode->image[j]);
            }
            free(imageNode->image);
            free(imageNode);
            fclose(fp);
            return NULL;
        }
        for (int j = 0; j < imageNode->cols; j++) {
            fscanf(fp, "%u %u %u", &R, &G, &B);
            imageNode->image[i][j].R = (uint8_t)R;
            imageNode->image[i][j].G = (uint8_t)G;
            imageNode->image[i][j].B = (uint8_t)B;
        }
    }
    fclose(fp);
    return imageNode;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    if (image) {
        printf("P3\n");
        printf("%u %u\n", image->cols, image->rows);
        printf("255\n");

        for (int i = 0; i < image->rows; i++) {
            for (int j = 0; j < image->cols; j++) {
                printf("%3u %3u %3u   ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
            }
            printf("\n");
        }
    }
}

// Frees an image
void freeImage(Image *image)
{
    if (image) {
        if (image->image) {
            for (int i = 0; i < image->rows; i++) {
                if (image->image[i]) {
                    free(image->image[i]);
                }
            }
            free(image->image);
        }
        free(image);
    }
}