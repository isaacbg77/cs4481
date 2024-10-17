#include "generate_huffman_nodes.h"

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array)
{
    struct node *huffman_nodes = (struct node *) malloc (number_of_non_zero_values_in_the_frequency_array-1 * sizeof(struct node));

    for (int i = 0; i < number_of_non_zero_values_in_the_frequency_array-1; i++) {
        struct node nextNode;
        nextNode.first_value = -1;
        nextNode.second_value = -1;

        for (int j = 0; j <= max_gray_value; j++) {
            long int freq = pixel_frequency[j];
            
            if (freq > 0) {
                if (nextNode.first_value == -1) {
                    nextNode.first_value = j;
                }
                else if (nextNode.second_value == -1) {
                    nextNode.second_value = j;
                }
                else if (freq < pixel_frequency[nextNode.first_value]) {
                    nextNode.first_value = nextNode.second_value;
                    nextNode.second_value = j;
                }
                else if (freq < pixel_frequency[nextNode.second_value]) {
                    nextNode.second_value = j;
                }
            }
        }

        pixel_frequency[nextNode.first_value] += pixel_frequency[nextNode.second_value];
        pixel_frequency[nextNode.second_value] = -1;
        
        huffman_nodes[i] = nextNode;
    }

    return huffman_nodes;
}
