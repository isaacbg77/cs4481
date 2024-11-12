#include "lz77_encoding_function.h"

int main(int argc, char *argv[])
{
    // Check for 2 input args
    if (argc != 3) {
        printf("Incorrect number of arguments. Please enter the input image file name and search buffer size.\n");
        return 0;
    }

    // Store input args
    char *input_pgm_image_name = argv[1];
    unsigned int buffer_size = atoi(argv[2]);

    // Init clock times and other data
    clock_t start, end;
    float avg_offset, std_offset, avg_length, std_length;

    // Encode image and record running time
    start = clock();
    Encode_Using_LZ77(input_pgm_image_name, buffer_size, &avg_offset, &std_offset, &avg_length, &std_length);
    end = clock();

    // Output calculated values and running time
    printf("Offset Average: %f\n", avg_offset);
    printf("Offset Standard Deviation: %f\n", std_offset);
    printf("Match Length Average: %f\n", avg_length);
    printf("Match Length Standard Deviation: %f\n", std_length);
    printf("Compression Time: %fs\n", ((double) end - start) / CLOCKS_PER_SEC);

    return 0;
}
