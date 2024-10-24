#ifndef HUFFMAN_DECODE_IMAGE_H
#define HUFFMAN_DECODE_IMAGE_H

#include <string.h>

#include "libpnm.h"
#include "generate_huffman_nodes.h"

struct tree_node
{
    int value;
    int left;
    int right;
};

struct PGM_Image *huffman_decode_image(int, int, int, int, struct node*, long int, unsigned char*);

#endif
