#include "generate_pixel_frequency.h"
#include "generate_huffman_nodes.h"
#include "huffman_encode_image.h"
#include "store_huffman_encoded_data.h"

int main(int argc, char *argv[])
{
    // Check for 2 input args
    if (argc != 3) {
        printf("Incorrect number of arguments. Please enter the input image file name and an output file name.\n");
        return 0;
    }

    // Store input args
    char *input_pgm_image_name = argv[1];
    char *output_file_name = argv[2];

    // Load input pgm image
    struct PGM_Image input_pgm_image;
    if (load_PGM_Image(&input_pgm_image, input_pgm_image_name) < 0) {
        printf("Failed to load input image with name: %s\n", input_pgm_image_name);
        return 0;
    }

    // Generate pixel frequencies
    int number_of_non_zero_values_in_the_frequency_array;
    long int *frequencies = generate_pixel_frequency(
        &input_pgm_image,
        &number_of_non_zero_values_in_the_frequency_array
    );

    // Generate huffman nodes
    struct node *huffman_nodes = generate_huffman_nodes(
        frequencies,
        input_pgm_image.maxGrayValue,
        number_of_non_zero_values_in_the_frequency_array
    );

    // Encode image
    long int length_of_encoded_image;
    unsigned char *encoded_data = huffman_encode_image(
        &input_pgm_image,
        huffman_nodes,
        number_of_non_zero_values_in_the_frequency_array-1,
        &length_of_encoded_image
    );

    // Write encoded image data to a file
    store_huffman_encoded_data(
        output_file_name,
        input_pgm_image.width,
        input_pgm_image.height,
        input_pgm_image.maxGrayValue,
        number_of_non_zero_values_in_the_frequency_array-1,
        huffman_nodes,
        length_of_encoded_image,
        encoded_data
    );

    // Free allocated memory
    free(frequencies);
    free(huffman_nodes);
    free(encoded_data);

    return 0;
}
