#include "read_huffman_encoded_data.h"

unsigned char *read_huffman_encoded_data(
    char *compressed_file_name_ptr,
    int *image_width,
    int *image_height,
    int *max_gray_value,
    int *number_of_nodes,
    struct node **huffman_node,
    long int *length_of_encoded_image_array
)
{
    // Open compressed file
    FILE *file = fopen(compressed_file_name_ptr, "r");
    if (file == NULL) {
        printf("Could not open compressed file: %s\n", compressed_file_name_ptr);
        return NULL;
    }

    // Read header values
    fscanf(file, "%d\n", image_width);
    fscanf(file, "%d\n", image_height);
    fscanf(file, "%d\n", max_gray_value);
    fscanf(file, "%d\n", number_of_nodes);

    // Read huffman node data
    *huffman_node = (struct node *) calloc (*number_of_nodes, sizeof(struct node));
    fread(*huffman_node, sizeof(struct node), *number_of_nodes, file);

    // Read encoded image data
    fscanf(file, "%li\n", length_of_encoded_image_array);
    unsigned char *encoded_data = (unsigned char *) calloc (*length_of_encoded_image_array, sizeof(unsigned char));
    fread(encoded_data, sizeof(unsigned char), *length_of_encoded_image_array, file);

    // Close file and return encoded data
    fclose(file);
    return encoded_data;
}
