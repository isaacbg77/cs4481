#include "huffman_encode_image.h"

unsigned char *huffman_encode_image(
    struct PGM_Image *input_pgm_image,
    struct node *huffman_node,
    int number_of_nodes,
    long int *length_of_encoded_image_array
)
{
    // Initialize codes and code lengths
    unsigned char huffman_codes[input_pgm_image->maxGrayValue+1][32];
    int code_lengths[input_pgm_image->maxGrayValue+1];
    int largest_code_length = 0;

    // Initialize all array values to 0
    memset(huffman_codes, 0, (input_pgm_image->maxGrayValue + 1) * 32 * sizeof(unsigned char));
    memset(code_lengths, 0, (input_pgm_image->maxGrayValue + 1) * sizeof(unsigned char));

    for (int i = number_of_nodes-1; i >= 0; i--) {
        // Get next huffman node at the code length for its first value
        struct node next_node = huffman_node[i];
        int code_length = code_lengths[next_node.first_value];
        int code_length_bytes = code_length / 8;
        unsigned char next_bit = (code_length % 8);

        // If second value exists
        if (next_node.second_value >= 0) {
            // Copy code from first value to second value
            for (int j = 0; j <= code_length_bytes; j++) {
                huffman_codes[next_node.second_value][j] = huffman_codes[next_node.first_value][j];
            }

            // Append 0 to first code and 1 to second code
            huffman_codes[next_node.first_value][code_length_bytes] &= ~(1 << (7 - next_bit));
            huffman_codes[next_node.second_value][code_length_bytes] |= 1 << (7 - next_bit);

            // Update code lengths
            code_lengths[next_node.first_value]++;
            code_lengths[next_node.second_value] = code_lengths[next_node.first_value];
        }
        // If second value does not exist, simply add one to first code length (which will be just 0)
        else
            code_lengths[next_node.first_value]++;

        // Keep track of longest code
        if (code_lengths[next_node.first_value] > largest_code_length)
            largest_code_length = code_lengths[next_node.first_value];
    }

    // Get image dimensions
    int image_width = input_pgm_image->width;
    int image_height = input_pgm_image->height;

    // Allocate space for encoded image data
    unsigned char *encoded_data = (unsigned char *) calloc (((int)largest_code_length/8 + 1) * image_width * image_height, sizeof(unsigned char));
    long int byte_pos = 0;
    unsigned char bit_pos = 0;

    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            // Pixel value
            unsigned char gray_value = input_pgm_image->image[i][j];

            // Get code for pixel value
            unsigned char *code = huffman_codes[gray_value];
            int code_length = code_lengths[gray_value];
            int code_bit_pos = 0;

            // For each bit in code
            while (code_bit_pos < code_length) {
                // Extract the next bit
                unsigned char next_bit = code[code_bit_pos/8] & 1 << (7 - code_bit_pos % 8);
                next_bit = next_bit >> (7 - code_bit_pos % 8);

                // Add extracted bit to encoded data
                encoded_data[byte_pos] |= next_bit << (7 - bit_pos);

                // Increment bit/byte positions
                bit_pos++;
                code_bit_pos++;
                if (bit_pos == 8) {
                    byte_pos++;
                    bit_pos = 0;
                }
            }
        }
    }

    // Store length of encoded image
    if (bit_pos == 0)
        *length_of_encoded_image_array = byte_pos;
    else
        *length_of_encoded_image_array = byte_pos + 1;

    return encoded_data;
}
