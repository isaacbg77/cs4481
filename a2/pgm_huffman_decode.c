#include "read_huffman_encoded_data.h"
#include "huffman_decode_image.h"

int main(int argc, char *argv[])
{
    // Check for 2 input args
    if (argc != 3) {
        printf("Incorrect number of arguments. Please enter the compressed image file name and an output file name.\n");
        return 0;
    }

    // Store input args
    char *compressed_pgm_image_name = argv[1];
    char *output_file_name = argv[2];

    // Read encoded data
    int image_width;
    int image_height;
    int max_gray_value;
    int number_of_nodes;
    struct node *huffman_nodes;
    long int length_of_encoded_image_array;

    unsigned char *encoded_image = read_huffman_encoded_data(
        compressed_pgm_image_name,
        &image_width,
        &image_height,
        &max_gray_value,
        &number_of_nodes,
        &huffman_nodes,
        &length_of_encoded_image_array
    );

    // Decode compressed image
    struct PGM_Image *decoded_image = huffman_decode_image(
        image_width,
        image_height,
        max_gray_value,
        number_of_nodes,
        huffman_nodes,
        length_of_encoded_image_array,
        encoded_image
    );

    // Save decoded image to file
    if (save_PGM_Image(decoded_image, output_file_name, true) < 0) {
        printf("Failed to save uncompressed image: %s\n", output_file_name);
        return 0;
    }

    // Free allocated memory
    free(huffman_nodes);
    free(encoded_image);
    free(decoded_image);

    return 0;
}
