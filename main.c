#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vader_lexicon.h"

// Main function which runs the program and displays results to the console
int main(int argc, char *argv[]) {

    // Error message for invalid number of parameter inputs
    if (argc != 3) {
        // Print the usage of how to correctly compile the program
        fprintf(stderr, "Usage: %s <vader_lexicon.txt> <input_file.txt>\n", argv[0]);
        return 1;
    }

    // Declare structure for lexicon and variable for dimension
    struct words *lexicon;
    int lexicon_dimension;

    // Read the lexicon file and populate the structure
    if (!read_vader_file(argv[1], &lexicon, &lexicon_dimension)) {
        return 1;
    }

    // open the input file in read mode
    FILE *input_file = fopen(argv[2], "r");
    // Check if file can be read
    if (!input_file) {
        // Print error message
        fprintf(stderr, "Error opening file %s\n", argv[2]);
        return 1;
    }

    // Format print for the header on the console
    printf("            string sample                                                                            score\n");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    // Set size for line variable
    char line[256];

    // Read each line of input file
    while (fgets(line, sizeof(line), input_file)) {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = 0;
        // Calculate the average sentiment score and print to console with formatting
        float score = calculate_sentiment_score(line, lexicon, lexicon_dimension);
        printf("%-85s %20.2f\n", line, score);
    }

    // Close the file
    fclose(input_file);

    // Free the memory for lexicon structure as well as for lexicon dimension
    free_lexicon(lexicon, lexicon_dimension);

    return 0;
}