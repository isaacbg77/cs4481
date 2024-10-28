#include "generate_huffman_nodes.h"

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array)
{
    // Huffman nodes array
    struct node *huffman_nodes;

    // If there is at least 2 distinct pixel values (image is not flat)
    if (number_of_non_zero_values_in_the_frequency_array > 1) {
        // Allocate space for nodes
        huffman_nodes = (struct node *) calloc (number_of_non_zero_values_in_the_frequency_array-1, sizeof(struct node));

        for (int i = 0; i < number_of_non_zero_values_in_the_frequency_array-1; i++) {
            // Initialize new node
            struct node next_node;
            next_node.first_value = -1;
            next_node.second_value = -1;

            for (int j = 0; j <= max_gray_value; j++) {
                long int freq = pixel_frequency[j];

                // If frequency is non-zero
                if (freq > 0) {
                    // Set as first value if does not exist
                    if (next_node.first_value == -1) {
                        next_node.first_value = j;
                    }
                    // Set as second value if does not exist
                    else if (next_node.second_value == -1) {
                        next_node.second_value = j;
                    }
                    // If less than first value, shift second value to first and update second value
                    else if (freq < pixel_frequency[next_node.first_value]) {
                        next_node.first_value = next_node.second_value;
                        next_node.second_value = j;
                    }
                    // If less than second value, update second value
                    else if (freq < pixel_frequency[next_node.second_value]) {
                        next_node.second_value = j;
                    }
                }
            }

            // Merge second frequency into first frequency and remove second frequency from array
            pixel_frequency[next_node.first_value] += pixel_frequency[next_node.second_value];
            pixel_frequency[next_node.second_value] = -1;

            // Add node to array
            huffman_nodes[i] = next_node;
        }
    }
    // If only 1 distinct pixel value (image is flat)
    else {
        // Allocate space for 1 node
        huffman_nodes = (struct node *) calloc (1, sizeof(struct node));
        struct node next_node;

        // Find non-zero pixel frequency
        for (int i = 0; i <= max_gray_value; i++) {
            if (pixel_frequency[i] > 0) {
                next_node.first_value = i;
                break;
            }
        }

        // Add single node to array
        next_node.second_value = -1;
        huffman_nodes[0] = next_node;
    }

    return huffman_nodes;
}
