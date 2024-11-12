#include "lz77_decoding_function.h"

void Decode_Using_LZ77(char *in_compressed_filename_Ptr)
{
    // Open compressed file
    FILE *file = fopen(in_compressed_filename_Ptr, "r");
    if (file == NULL) {
        printf("Could not open compressed file: %s\n", in_compressed_filename_Ptr);
        return;
    }

    // Read file header (number of tokens)
    int width, height, maxGray, num_tokens;
    fscanf(file, "%d %d %d %d\n", &width, &height, &maxGray, &num_tokens);

    // Read encoded data
    unsigned int *offsets = (unsigned int *) calloc (num_tokens, sizeof(unsigned int));
    unsigned int *lengths = (unsigned int *) calloc (num_tokens, sizeof(unsigned int));
    unsigned char *next_symbols = (unsigned char *) calloc (num_tokens, sizeof(unsigned char));

    fread(offsets, sizeof(unsigned int), num_tokens, file);
    fscanf(file, "\n");
    fread(lengths, sizeof(unsigned int), num_tokens, file);
    fscanf(file, "\n");
    fread(next_symbols, sizeof(unsigned char), num_tokens, file);
    fscanf(file, "\n");

    // Close file
    fclose(file);

    // ---------------------------------------------------------------------------------------------------

    unsigned char *symbols = (unsigned char *) calloc (width*height, sizeof(unsigned char));
    int symbol_pos = 0;

    for (int i = 0; i < num_tokens; i++) {
        unsigned int offset = offsets[i];
        unsigned int length = lengths[i];
        unsigned char next = next_symbols[i];

        int start = symbol_pos - offset;
        for (int j = start; j < start + length; j++) {
            symbols[symbol_pos] = symbols[j];
            symbol_pos++;
        }

        symbols[symbol_pos] = next;
        symbol_pos++;
    }

    // ---------------------------------------------------------------------------------------------------

    struct PGM_Image out_image;
    if (create_PGM_Image(&out_image, width, height, maxGray) < 0) {
        printf("Failed to create PGM image.\n");
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            out_image.image[i][j] = symbols[i * width + j];
        }
    }

    char file_name[100];
    sprintf(file_name, "%s.pgm", in_compressed_filename_Ptr);

    if (save_PGM_Image(&out_image, file_name, true) < 0) {
        printf("Failed to save decompressed image: %s\n", in_compressed_filename_Ptr);
        return;
    }

    // Free allocated memory
    free_PGM_Image(&out_image);
    free(offsets);
    free(lengths);
    free(next_symbols);
    free(symbols);
}
