#include <stdlib.h>
#include <stdio.h>

#include "libpnm.h" // PNM library

int round_positive(float f) {
    if (f < 0) return 0;
    return (int) (f + 0.5);
}

void generate_pbm_image(unsigned int width, unsigned int height, char *name, bool raw) {
    // Create empty pbm image
    struct PBM_Image pbmImage;
    if (create_PBM_Image(&pbmImage, width, height) < 0) {
        printf("Failed to initialize pbm image.\n");
        return;
    }

    int larger = width >= height ? width : height;
    int smaller = width >= height ? height : width;

    float lineStepSize = (float) larger / smaller;
    float lineStep1 = (float) larger / 4;
    float lineStep2 = (float) larger * 3/4 - 1;

    for (int i = 0; i < smaller; i++) {
        for (int j = 0; j < larger; j++) {
            if (i < smaller/4 || i >= smaller*3/4 || j < larger/4 || j >= larger*3/4) {
                larger == width ? (pbmImage.image[i][j] = 1) : (pbmImage.image[j][i] = 1);
            }
            else {
                if (j >= round_positive(lineStep1) && j < round_positive(lineStep1 + lineStepSize)) {
                    larger == width ? (pbmImage.image[i][j] = 1) : (pbmImage.image[j][i] = 1);
                }
                else if (j > round_positive(lineStep2 - lineStepSize) && j <= round_positive(lineStep2)) {
                    larger == width ? (pbmImage.image[i][j] = 1) : (pbmImage.image[j][i] = 1);
                }
            }
        }

        if (i >= smaller/4 && i < smaller*3/4) {
            lineStep1 += lineStepSize;
            lineStep2 -= lineStepSize;
        }
    }

    // Save pbm image
    if (save_PBM_Image(&pbmImage, name, raw) < 0) {
        printf("Failed to save pbm image.\n");
        return;
    }

    // Free allocated memory
    free_PBM_Image(&pbmImage);
}

int main(int argc, char *argv[])
{
    // Check for 5 input args
    if (argc != 6) {
        printf("Incorrect number of arguments. Please enter 5 arguments.\n");
        return EXIT_FAILURE;
    }

    // Read in args
    unsigned int type = atoi(argv[1]);
    unsigned int width = atoi(argv[2]);
    unsigned int height = atoi(argv[3]);
    char *name = argv[4];
    bool raw = atoi(argv[5]);

    // Check image width
    if (type == 1 || type == 2) {
        if (width < 4 || width % 4 != 0) {
            printf("Image width of pbm and pgm images must be divisible by 4 and equal to 4 or greater.\n");
            return EXIT_FAILURE;
        }
    }
    else if (type == 3) {
        if (width < 6 || width % 6 != 0) {
            printf("Image width of ppm images must be divisible by 6 and equal to 6 or greater.\n");
            return EXIT_FAILURE;
        }
    }
    // Check image height
    if (height < 4 || height % 4 != 0) {
        printf("Image height must be divisible by 4 and equal to 4 or greater.\n");
        return EXIT_FAILURE;
    }

    // Generate image based on type code
    if (type == 1) {
        generate_pbm_image(width, height, name, raw);
    }

    return EXIT_SUCCESS;
}
