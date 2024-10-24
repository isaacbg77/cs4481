#include "mean_absolute_error.h"

float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr)
{
    struct PGM_Image image1;
    if (load_PGM_Image(&image1, file_name_1_ptr) < 0) {
        printf("Failed to load image with name: %s\n", file_name_1_ptr);
        return -1;
    }

    struct PGM_Image image2;
    if (load_PGM_Image(&image2, file_name_2_ptr) < 0) {
        printf("Failed to load image with name: %s\n", file_name_2_ptr);
        return -1;
    }

    if (image1.width != image2.width || image1.height != image2.height) {
        printf("Error: provided images are not the same size.\n");
        return -1;
    }

    if (image1.maxGrayValue != image2.maxGrayValue) {
        struct PGM_Image *smaller = image1.maxGrayValue < image2.maxGrayValue ? &image1 : &image2;
        struct PGM_Image *larger = image1.maxGrayValue > image2.maxGrayValue ? &image1 : &image2;
        int scale_factor = larger->maxGrayValue / smaller->maxGrayValue;

        for (int i = 0; i < smaller->height; i++) {
            for (int j = 0; j < smaller->width; j++) {
                smaller->image[i][j] *= scale_factor;
            }
        }
        smaller->maxGrayValue = larger->maxGrayValue;
    }

    int sum = 0;
    for (int i = 0; i < image1.height; i++) {
        for (int j = 0; j < image1.width; j++) {
            sum += abs(image1.image[i][j] - image2.image[i][j]);
        }
    }

    return (float) sum / (image1.width * image1.height);
}
