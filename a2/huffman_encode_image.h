#ifndef HUFFMAN_ENCODE_IMAGE_H
#define HUFFMAN_ENCODE_IMAGE_H

#include "libpnm.h"
#include "generate_huffman_nodes.h"

unsigned char *huffman_encode_image(struct PGM_Image*, struct node*, int, long int*);

#endif
