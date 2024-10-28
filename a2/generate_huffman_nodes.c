#include "generate_huffman_nodes.h"

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array)
{
    struct node *huffman_nodes;

    if (number_of_non_zero_values_in_the_frequency_array > 1) {
        huffman_nodes = (struct node *) calloc (number_of_non_zero_values_in_the_frequency_array-1, sizeof(struct node));

        for (int i = 0; i < number_of_non_zero_values_in_the_frequency_array-1; i++) {
            struct node next_node;
            next_node.first_value = -1;
            next_node.second_value = -1;

            for (int j = 0; j <= max_gray_value; j++) {
                long int freq = pixel_frequency[j];

                if (freq > 0) {
                    if (next_node.first_value == -1) {
                        next_node.first_value = j;
                    }
                    else if (next_node.second_value == -1) {
                        next_node.second_value = j;
                    }
                    else if (freq < pixel_frequency[next_node.first_value]) {
                        next_node.first_value = next_node.second_value;
                        next_node.second_value = j;
                    }
                    else if (freq < pixel_frequency[next_node.second_value]) {
                        next_node.second_value = j;
                    }
                }
            }

            pixel_frequency[next_node.first_value] += pixel_frequency[next_node.second_value];
            pixel_frequency[next_node.second_value] = -1;

            huffman_nodes[i] = next_node;
        }
    }
    else {
        huffman_nodes = (struct node *) calloc (1, sizeof(struct node));
        struct node next_node;

        for (int i = 0; i <= max_gray_value; i++) {
            if (pixel_frequency[i] > 0) {
                next_node.first_value = i;
                break;
            }
        }
        next_node.second_value = -1;
        huffman_nodes[0] = next_node;
    }

    return huffman_nodes;
}
