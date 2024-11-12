#include "lz77_decoding_function.h"

int main(int argc, char *argv[])
{
    // Check for 1 input arg
    if (argc != 2) {
        printf("Incorrect number of arguments. Please enter the input compressed file name.\n");
        return 0;
    }

    // Store input args
    char *in_file_name = argv[1];

    clock_t start, end;

    start = clock();
    Decode_Using_LZ77(in_file_name);
    end = clock();

    printf("Decompression Time: %fs\n", ((double) end - start) / CLOCKS_PER_SEC);

    return 0;
}
