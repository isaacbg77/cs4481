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
    // Initialize huffman tree
    struct tree_node huffman_tree[2*number_of_nodes + 1];
    int leaves[max_gray_value + 1];
    memset(leaves, 0, (max_gray_value + 1) * sizeof(int));

    // Initialize root tree node
    struct tree_node root;
    root.value = root.left = root.right = -1;
    huffman_tree[0] = root;

    // Initialize next available space in tree array
    int next_index = 1;

    for (int i = number_of_nodes-1; i >= 0; i--) {
        // Get next node and position of first node value in tree
        struct node next_node = huffman_node[i];
        int leaf = leaves[next_node.first_value];

        // Replace leaf with new internal node
        huffman_tree[leaf].value = -1;
        huffman_tree[leaf].left = next_index;
        huffman_tree[leaf].right = next_index + 1;

        // Add left child with first node value
        struct tree_node left_child;
        left_child.value = next_node.first_value;
        left_child.left = left_child.right = -1;
        huffman_tree[next_index] = left_child;

        leaves[next_node.first_value] = next_index;
        next_index++;

        // Add right child with second node value (if exists)
        if (next_node.second_value >= 0) {
            struct tree_node right_child;
            right_child.value = next_node.second_value;
            right_child.left = right_child.right = -1;
            huffman_tree[next_index] = right_child;

            leaves[next_node.second_value] = next_index;
            next_index++;
        }
    }

    // Allocate space for decoded image
    struct PGM_Image *decoded_image = (struct PGM_Image *) calloc (1, sizeof(struct PGM_Image));
    if (create_PGM_Image(decoded_image,image_width, image_height, max_gray_value) < 0) {
        printf("Failed to create new PGM image.\n");
        return NULL;
    }

    long int byte_pos = 0;
    unsigned char bit_pos = 0;

    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            // Always start at root
            struct tree_node current_node = huffman_tree[0];

            // While not a leaf
            while (current_node.value < 0) {
                // Read next bit from encoded data
                unsigned char next_bit = encoded_image[byte_pos] & 1 << (7 - bit_pos);

                // Traverse to a child node
                if (next_bit == 0) {
                    current_node = huffman_tree[current_node.left];
                }
                else {
                    current_node = huffman_tree[current_node.right];
                }

                // Increment bit/byte position
                bit_pos++;
                if (bit_pos == 8) {
                    byte_pos++;
                    bit_pos = 0;
                }
            }

            // Set pixel value in decoded image from leaf node
            decoded_image->image[i][j] = current_node.value;
        }
    }

    // Return decompressed image
    return decoded_image;
}
