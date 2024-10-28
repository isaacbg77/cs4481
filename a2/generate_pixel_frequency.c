#include "generate_pixel_frequency.h"

long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, int *number_of_non_zero_values_in_the_frequency_array)
{
    // Allocate memory for frequency array
    long int *frequencies = (long int *) calloc (input_pgm_image->maxGrayValue + 1, sizeof(long int));

    // Update frequency value for each pixel in image
    for (int i = 0; i < input_pgm_image->height; i++) {
        for (int j = 0; j < input_pgm_image->width; j++) {
            frequencies[input_pgm_image->image[i][j]]++;
        }
    }

    // Calculate the number of non-zero frequency values
    for (int i = 0; i <= input_pgm_image->maxGrayValue; i++) {
        if (frequencies[i] > 0)
            (*number_of_non_zero_values_in_the_frequency_array)++;
    }

    return frequencies;
}
