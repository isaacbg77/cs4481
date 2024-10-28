#include "mean_absolute_error.h"

int main(int argc, char *argv[])
{
    // Check for 2 input args
    if (argc != 3) {
        printf("Incorrect number of arguments. Please enter two image file names.\n");
        return 0;
    }

    // Store input args
    char *image1_file_name = argv[1];
    char *image2_file_name = argv[2];

    float mae = mean_absolute_error(image1_file_name, image2_file_name);
    if (mae >= 0) {
        printf("Mean Absolute Error: %f (%s, %s)\n", mae, image1_file_name, image2_file_name);
    }

    return 0;
}
