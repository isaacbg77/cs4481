#ifndef GENERATE_HUFFMAN_NODES_H
#define GENERATE_HUFFMAN_NODES_H

#include <stdlib.h>

struct node
{
    int first_value;
    int second_value;
};

struct node *generate_huffman_nodes(long int*, int, int);

#endif
