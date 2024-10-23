#include "store_huffman_encoded_data.h"

void store_huffman_encoded_data(
    char *compressed_file_name_ptr,
    int image_width,
    int image_height,
    int max_gray_value,
    int number_of_nodes,
    struct node *huffman_node,
    long int length_of_encoded_image_array,
    unsigned char *encoded_image
)
{
    FILE *file = fopen(compressed_file_name_ptr, "w");
    if (file == NULL) {
        printf("Could not write to compressed file.\n");
        return;
    }

    fprintf(file, "%d\n", image_width);
    fprintf(file, "%d\n", image_height);
    fprintf(file, "%d\n", max_gray_value);

    fprintf(file, "%d\n", number_of_nodes);
    fwrite(huffman_node, sizeof(struct node), number_of_nodes, file);
    fprintf(file, "\n");

    fprintf(file, "%li\n", length_of_encoded_image_array);
    fwrite(encoded_image, sizeof(unsigned char), length_of_encoded_image_array, file);
    fprintf(file, "\n");

    fclose(file);
}
