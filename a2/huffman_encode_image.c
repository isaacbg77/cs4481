#include "huffman_encode_image.h"

unsigned char *huffman_encode_image(
    struct PGM_Image *input_pgm_image,
    struct node *huffman_node,
    int number_of_nodes,
    long int *length_of_encoded_image_array
)
{
    unsigned char huffman_codes[input_pgm_image->maxGrayValue+1][32];
    int code_lengths[input_pgm_image->maxGrayValue+1];
    int largest_code_length = 0;

    for (int i = number_of_nodes-1; i >= 0; i--) {
        struct node nextNode = huffman_node[i];
        int code_length = code_lengths[nextNode.first_value];
        int code_length_bytes = code_length / 8;
        unsigned char next_bit = (code_length % 8);

        for (int j = 0; j <= code_length_bytes; j++) {
            huffman_codes[nextNode.second_value][j] = huffman_codes[nextNode.first_value][j];
        }

        huffman_codes[nextNode.first_value][code_length_bytes] &= ~(1 << (7 - next_bit));
        huffman_codes[nextNode.second_value][code_length_bytes] |= 1 << (7 - next_bit);

        code_lengths[nextNode.first_value]++;
        code_lengths[nextNode.second_value]++;

        if (code_lengths[nextNode.first_value] > largest_code_length)
            largest_code_length = code_lengths[nextNode.first_value];
    }

    int image_width = input_pgm_image->width;
    int image_height = input_pgm_image->height;

    unsigned char *encoded_data = (unsigned char *) calloc (((int)largest_code_length/8 + 1) * image_width * image_height, sizeof(unsigned char));
    long int byte_pos = 0;
    unsigned char bit_pos = 0;

    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            unsigned char grayValue = input_pgm_image->image[i][j];
            
            unsigned char *code = huffman_codes[grayValue];
            int code_length = code_lengths[grayValue];
            int code_bit_pos = 0;

            while (code_bit_pos < code_length) {
                unsigned char next_bit = code_bit_pos % 8;
                encoded_data[byte_pos] |= (code[code_bit_pos/8] & 1 << (7 - next_bit));

                bit_pos++;
                code_bit_pos++;

                if (bit_pos == 8) {
                    byte_pos++;
                    bit_pos = 0;
                }
            }
        }
    }

    if (bit_pos == 0)
        *length_of_encoded_image_array = byte_pos;
    else
        *length_of_encoded_image_array = byte_pos + 1;

    return encoded_data;
}
