#include "lz77_encoding_function.h"

// Comparator function used for sorting
int compare_unsigned_ints(const void *a, const void *b)
{
    int int1 = *(const unsigned int *)a;
    int int2 = *(const unsigned int *)b;
    return (int1 - int2);
}

// Function used to calculate mean and standard deviation from a set of values
void calculate_std(unsigned int *values, int n, float *mean, float *std)
{
    // Mean
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += values[i];
    }
    *mean = sum / n;

    // Standard deviation
    float variance = 0;
    for (int i = 0; i < n; i++) {
        variance += pow(values[i] - *mean, 2);
    }
    variance /= n;
    *std = sqrt(variance);
}

void Encode_Using_LZ77(
    char *in_PGM_filename_Ptr,
    unsigned int searching_buffer_size,
    float *avg_offset_Ptr,
    float *std_offset_Ptr,
    float *avg_length_Ptr,
    float *std_length_Ptr
)
{
    // Load in image
    struct PGM_Image in_image;
    if (load_PGM_Image(&in_image, in_PGM_filename_Ptr) < 0) {
        printf("Failed to load input image with name: %s\n", in_PGM_filename_Ptr);
        return;
    }
    int width = in_image.width;
    int height = in_image.height;
    int maxGray = in_image.maxGrayValue;

    // Convert image data to a 1D symbol array
    unsigned char *symbols = (unsigned char *) calloc (width*height, sizeof(unsigned char));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            symbols[i * width + j] = in_image.image[i][j];
        }
    }

    // Free image
    free_PGM_Image(&in_image);

    // ---------------------------------------------------------------------------------------------------

    // Allocate space for token data
    unsigned int num_tokens = 0;
    unsigned int *offsets = (unsigned int *) calloc (width*height, sizeof(unsigned int));
    unsigned int *lengths = (unsigned int *) calloc (width*height, sizeof(unsigned int));
    unsigned char *next_symbols = (unsigned char *) calloc (width*height, sizeof(unsigned char));

    for (int i = 0; i < width*height; i++) {
        // Calculate pointer for the end of the search buffer
        int search_buffer_end = i - 1 - searching_buffer_size;
        if (search_buffer_end < 0) {
            search_buffer_end = 0;
        }

        // Init next token values
        unsigned int longest_match_offset = 0;
        unsigned int longest_match_length = 0;
        unsigned char longest_match_next = symbols[i];

        if (i > 0) {
            for (int s = i-1; s >= search_buffer_end; s--) {
                // Search buffer until match is found
                if (symbols[i] == symbols[s]) {
                    // Init match values
                    unsigned int match_offset = i - s;
                    unsigned int match_length = 0;
                    unsigned int match_index = 0;

                    // Calculate length of match
                    while (symbols[i + match_index] == symbols[s + match_index]) {
                        match_index++;
                        match_length++;
                    }

                    // Store new longest match
                    if (match_length > longest_match_length) {
                        longest_match_offset = match_offset;
                        longest_match_length = match_length;
                        longest_match_next = symbols[i + match_index];
                    }
                }
            }
        }

        // Store next token
        offsets[num_tokens] = longest_match_offset;
        lengths[num_tokens] = longest_match_length;
        next_symbols[num_tokens] = longest_match_next;
        num_tokens++;

        // Increment pointer based on the longest match that was found
        i += longest_match_length;
    }

    // ---------------------------------------------------------------------------------------------------

    // Open new compressed file
    char compressed_file_name[100];
    sprintf(compressed_file_name, "%s.%u.lz", in_PGM_filename_Ptr, searching_buffer_size);
    FILE *file = fopen(compressed_file_name, "w");
    if (file == NULL) {
        printf("Could not write to compressed file.\n");
        return;
    }

    // Write file header
    fprintf(file, "%d %d %d %d\n", width, height, maxGray, num_tokens);

    // Write encoded data to file
    fwrite(offsets, sizeof(unsigned int), num_tokens, file);
    fprintf(file, "\n");
    fwrite(lengths, sizeof(unsigned int), num_tokens, file);
    fprintf(file, "\n");
    fwrite(next_symbols, sizeof(unsigned char), num_tokens, file);
    fprintf(file, "\n");

    // Close file
    fclose(file);

    // ---------------------------------------------------------------------------------------------------

    // Open file for offsets.csv
    char offsets_file_name[100];
    sprintf(offsets_file_name, "%s.%u.offsets.csv", in_PGM_filename_Ptr, searching_buffer_size);
    file = fopen(offsets_file_name, "w");
    if (file == NULL) {
        printf("Could not write to offsets.csv file.\n");
        return;
    }

    // Sort offsets array
    qsort(offsets, num_tokens, sizeof(unsigned int), compare_unsigned_ints);

    // Write offsets and frequencies to file
    for (int i = 0; i < num_tokens; i++) {
        unsigned int offset_value = offsets[i];
        unsigned int frequency = 0;

        fprintf(file, "%u,", offset_value);
        while (i < num_tokens && offsets[i] == offset_value) {
            frequency++;
            i++;
        }
        i--;
        fprintf(file, "%u\n", frequency);
    }

    // Close file
    fclose(file);

    // ---------------------------------------------------------------------------------------------------

    // Open file for lengths.csv
    char lengths_file_name[100];
    sprintf(lengths_file_name, "%s.%u.lengths.csv", in_PGM_filename_Ptr, searching_buffer_size);
    file = fopen(lengths_file_name, "w");
    if (file == NULL) {
        printf("Could not write to lengths.csv file.\n");
        return;
    }

    // Sort lengths array
    qsort(lengths, num_tokens, sizeof(unsigned int), compare_unsigned_ints);

    // Write lengths and frequencies to file
    for (int i = 0; i < num_tokens; i++) {
        unsigned int length_value = lengths[i];
        unsigned int frequency = 0;

        fprintf(file, "%u,", length_value);
        while (i < num_tokens && lengths[i] == length_value) {
            frequency++;
            i++;
        }
        i--;
        fprintf(file, "%u\n", frequency);
    }

    // Close file
    fclose(file);

    // ---------------------------------------------------------------------------------------------------

    // Calculate mean and std for offsets and lengths
    calculate_std(offsets, num_tokens, avg_offset_Ptr, std_offset_Ptr);
    calculate_std(lengths, num_tokens, avg_length_Ptr, std_length_Ptr);

    // Free allocated memory
    free(symbols);
    free(offsets);
    free(lengths);
    free(next_symbols);
}
