#include <stdlib.h>
#include <stdio.h>

#include "libpnm.h" // PNM library

int round_float(float f) {
    return (int) (f < 0 ? f - 0.5 : f + 0.5);
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
                if (j >= round_float(lineStep1) && j < round_float(lineStep1 + lineStepSize)) {
                    larger == width ? (pbmImage.image[i][j] = 1) : (pbmImage.image[j][i] = 1);
                }
                else if (j > round_float(lineStep2 - lineStepSize) && j <= round_float(lineStep2)) {
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

void generate_pgm_image(unsigned int width, unsigned int height, char *name, bool raw) {
    int maxGray = 255;

    // Create empty pgm image
    struct PGM_Image pgmImage;
    if (create_PGM_Image(&pgmImage, width, height, maxGray) < 0) {
        printf("Failed to initialize pgm image.\n");
        return;
    }

    int larger = width >= height ? width : height;
    int smaller = width >= height ? height : width;

    float lineStepSize = (float) larger / smaller;
    float lineStep1 = (float) larger / 4;
    float lineStep2 = (float) larger * 3/4 - 1;

    float largerGrayLevel = maxGray;
    float largerGrayStepSize = (float) maxGray / larger * 4;

    float smallerGrayLevel = maxGray;
    float smallerGrayStepSize = (float) maxGray / smaller * 4;

    for (int i = 0; i < smaller; i++) {
        largerGrayLevel = maxGray;
        
        for (int j = 0; j < larger; j++) {
            // Draw black outer rectangle
            if (i < smaller/4 || i >= smaller*3/4 || j < larger/4 || j >= larger*3/4) {
                larger == width ? (pgmImage.image[i][j] = 0 ) : (pgmImage.image[j][i] = 0);
            }

            // Draw inside white rectangle
            else {
                if (i < smaller/2) {
                    if (j < round_float(lineStep1)) {
                        larger == width ? (pgmImage.image[i][j] = round_float(largerGrayLevel)) : (pgmImage.image[j][i] = round_float(largerGrayLevel));
                        largerGrayLevel -= largerGrayStepSize;
                    }
                    else if (j >= round_float(lineStep1) && j <= round_float(lineStep2)) {
                        larger == width ? (pgmImage.image[i][j] = round_float(smallerGrayLevel)) : (pgmImage.image[j][i] = round_float(smallerGrayLevel));
                    }
                    else if (j > round_float(lineStep2)) {
                        larger == width ? (pgmImage.image[i][j] = round_float(largerGrayLevel)) : (pgmImage.image[j][i] = round_float(largerGrayLevel));
                        largerGrayLevel += largerGrayStepSize;
                    }
                }
                else if (i >= smaller/2) {
                    if (j < round_float(lineStep2)) {
                        larger == width ? (pgmImage.image[i][j] = round_float(largerGrayLevel)) : (pgmImage.image[j][i] = round_float(largerGrayLevel));
                        largerGrayLevel -= largerGrayStepSize;
                    }
                    else if (j >= round_float(lineStep2) && j <= round_float(lineStep1)) {
                        larger == width ? (pgmImage.image[i][j] = round_float(smallerGrayLevel)) : (pgmImage.image[j][i] = round_float(smallerGrayLevel));
                    }
                    else if (j > round_float(lineStep1)) {
                        larger == width ? (pgmImage.image[i][j] = round_float(largerGrayLevel)) : (pgmImage.image[j][i] = round_float(largerGrayLevel));
                        largerGrayLevel += largerGrayStepSize;
                    }
                }
            }
        }

        if (i >= smaller/4 && i < smaller*3/4) {
            lineStep1 += lineStepSize;
            lineStep2 -= lineStepSize;

            if (i < smaller/2)
                smallerGrayLevel -= smallerGrayStepSize;
            else if (i >= smaller/2)
                smallerGrayLevel += smallerGrayStepSize;
        }
    }

    // Save pgm image
    if (save_PGM_Image(&pgmImage, name, raw) < 0) {
        printf("Failed to save pgm image.\n");
        return;
    }

    // Free allocated memory
    free_PGM_Image(&pgmImage);
}

void generate_ppm_image(unsigned int width, unsigned int height, char *name, bool raw) {
    int maxColor = 255;

    // Create empty ppm image
    struct PPM_Image ppmImage;
    if (create_PPM_Image(&ppmImage, width, height, maxColor) < 0) {
        printf("Failed to initialize ppm image.\n");
        return;
    }

    float colorGradientLevel = 0;
    float grayGradientLevel = 0;
    float gradientStepSize = (float) maxColor / height * 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i < height/2) {
                if (j < width/3) {
                    ppmImage.image[i][j][RED] = maxColor;
                    ppmImage.image[i][j][GREEN] = ppmImage.image[i][j][BLUE] = round_float(colorGradientLevel);
                }
                else if (j >= width/3 && j < width*2/3) {
                    ppmImage.image[i][j][GREEN] = maxColor;
                    ppmImage.image[i][j][RED] = ppmImage.image[i][j][BLUE] = round_float(maxColor - colorGradientLevel);

                }
                else if (j >= width*2/3) {
                    ppmImage.image[i][j][BLUE] = maxColor;
                    ppmImage.image[i][j][RED] = ppmImage.image[i][j][GREEN] = round_float(colorGradientLevel);
                }
            }
            else if (i >= height/2) {

                if (j < width/2) {
                    ppmImage.image[i][j][RED] = ppmImage.image[i][j][GREEN] = ppmImage.image[i][j][BLUE] = round_float(grayGradientLevel);
                }
                else if (j >= width/2) {
                    ppmImage.image[i][j][RED] = ppmImage.image[i][j][GREEN] = ppmImage.image[i][j][BLUE] = round_float(maxColor - grayGradientLevel);
                }
            }
        }

        if (i < height/2)
            colorGradientLevel = colorGradientLevel + gradientStepSize;
        else if (i >= height/2)
            grayGradientLevel = grayGradientLevel + gradientStepSize;
    }

    // Save ppm image
    if (save_PPM_Image(&ppmImage, name, raw) < 0) {
        printf("Failed to save ppm image.\n");
        return;
    }

    for (int color = RED; color <= BLUE; color++) {
        struct PGM_Image temp;

        create_PGM_Image(&temp, width, height, maxColor);
        copy_PPM_to_PGM(&ppmImage, &temp, color);

        char tempName[500];
        if (color == RED)
            sprintf(tempName, "%s_red.pgm", name);
        else if (color == GREEN)
            sprintf(tempName, "%s_green.pgm", name);
        else if (color == BLUE)
            sprintf(tempName, "%s_blue.pgm", name);

        save_PGM_Image(&temp, tempName, raw);
        free_PGM_Image(&temp);
    }

    // Free allocated memory
    free_PPM_Image(&ppmImage);
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
    else if (type == 2) {
        generate_pgm_image(width, height, name, raw);
    }
    else if (type == 3) {
        generate_ppm_image(width, height, name, raw);
    }

    return EXIT_SUCCESS;
}
