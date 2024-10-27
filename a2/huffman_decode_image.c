#include "huffman_decode_image.h"

struct PGM_Image *huffman_decode_image(
    int image_width,
    int image_height,
    int max_gray_value,
    int number_of_nodes,
    struct node *huffman_node,
    long int length_of_encoded_image_array,
    unsigned char *encoded_image
)
{
    struct tree_node huffman_tree[2*number_of_nodes + 1];
    int leaves[max_gray_value + 1];
    memset(leaves, 0, (max_gray_value + 1) * sizeof(int));

    struct tree_node root;
    root.value = root.left = root.right = -1;
    huffman_tree[0] = root;

    int next_index = 1;

    for (int i = number_of_nodes-1; i >= 0; i--) {
        struct node next_node = huffman_node[i];
        int leaf = leaves[next_node.first_value];

        huffman_tree[leaf].value = -1;
        huffman_tree[leaf].left = next_index;
        huffman_tree[leaf].right = next_index + 1;

        struct tree_node left_child;
        left_child.value = next_node.first_value;
        left_child.left = left_child.right = -1;
        huffman_tree[next_index] = left_child;

        struct tree_node right_child;
        right_child.value = next_node.second_value;
        right_child.left = right_child.right = -1;
        huffman_tree[next_index + 1] = right_child;
 
        leaves[next_node.first_value] = next_index;
        leaves[next_node.second_value] = next_index + 1;
        next_index += 2;
    }

    struct PGM_Image *decoded_image = (struct PGM_Image *) calloc (1, sizeof(struct PGM_Image));
    if (create_PGM_Image(decoded_image,image_width, image_height, max_gray_value) < 0) {
        printf("Failed to create new PGM image.\n");
        return NULL;
    }

    long int byte_pos = 0;
    unsigned char bit_pos = 0;

    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            struct tree_node current_node = huffman_tree[0];
            
            while (current_node.value < 0) {
                unsigned char next_bit = encoded_image[byte_pos] & 1 << (7 - bit_pos);

                if (next_bit == 0) {
                    current_node = huffman_tree[current_node.left];
                }
                else {
                    current_node = huffman_tree[current_node.right];
                }

                bit_pos++;
                if (bit_pos == 8) {
                    byte_pos++;
                    bit_pos = 0;
                }
            }

            decoded_image->image[i][j] = current_node.value;
        }
    }

    return decoded_image;
}
