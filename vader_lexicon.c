#include "vader_lexicon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Defining maximum length for line to be parsed and for the word
#define MAX_LINE_LENGTH 256
#define MAX_WORD_LENGTH 50

// Function which reads the "vader_lexicon.txt" file and stores its data within the defined "words" structure.
int read_vader_file(const char *filename, struct words **lexicon, int *lexicon_dimension) {

    // Opening the file in reading mode
    FILE *file = fopen(filename, "r");
    // Check if opening the file was unsucessful 
    if (!file) {
        // Print Error message indicating file opening was not sucessful
        fprintf(stderr, "Error opening file %s\n", filename);

        // Return 0 indicating failure in opening file
        return 0;
    }

    // Declare and initialize pointers for array size and memory allocation.
    *lexicon_dimension = 0;
    *lexicon = NULL;

    // Set the size of the line length to be parsed
    char line[MAX_LINE_LENGTH];

    //  Reading each line from the file
    while (fgets(line, sizeof(line), file)) {

        // Set the size of the max word length
        char word[MAX_WORD_LENGTH];

        // Declare variables for data to be stored from file
        float score, SD;
        int SIS_array[10];

        // Parse the line and retrieve the word, score, SD, and SIS_array
        if (sscanf(line, "%s %f %f [%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]",
                   word, &score, &SD,
                   &SIS_array[0], &SIS_array[1], &SIS_array[2], &SIS_array[3],
                   &SIS_array[4], &SIS_array[5], &SIS_array[6], &SIS_array[7],
                   &SIS_array[8], &SIS_array[9]) != 13) {
            continue;
        }

        // Allocate memory for the new lexicon entry
        struct words *temp = (struct words *)realloc(*lexicon, (*lexicon_dimension + 1) * sizeof(struct words));
        // Check if memory allocation was successful
        if (!temp) {
            // Print Error message if unsuccessful, close the file, and return 0
            fprintf(stderr, "Memory allocation error\n");
            fclose(file);
            return 0;
        }

        // Update the lexicon pointer
        *lexicon = temp;

        // Allocate memory for the word
        (*lexicon)[*lexicon_dimension].word = (char *)malloc((strlen(word) + 1) * sizeof(char));
        // Check if memory allocation is successful
        if (!((*lexicon)[*lexicon_dimension].word)) {
            // Print Error message if unsuccessful, close the file, and return 0
            fprintf(stderr, "Memory allocation error\n");
            fclose(file);
            return 0;
        }
        // Copy the word to the lexicon entry
        strcpy((*lexicon)[*lexicon_dimension].word, word);

        // Assign the score and Standard Deviation value to lexicon entry
        (*lexicon)[*lexicon_dimension].score = score;
        (*lexicon)[*lexicon_dimension].SD = SD;

        // Loop in order to assign values to the SIS array in the lexicon entry
        for (int i = 0; i < 10; i++) {
            (*lexicon)[*lexicon_dimension].SIS_array[i] = SIS_array[i];
        }

        //Increment the size of lexicon
        (*lexicon_dimension)++;
    }

    // Close file and return value of 1
    fclose(file);
    return 1;
}

// Function which calculates the sentiment score average
float calculate_sentiment_score(const char *sentence, struct words *lexicon, int lexicon_dimension) {

    // Declare and initialize variables for tracking score and word count
    float total_score = 0.0;
    int word_count = 0;

    // Create a duplicate of the input sentence
    char *processed_sentence = strdup(sentence);
    // Check if memory allocation is successful
    if (!processed_sentence) {
         // Print error message if unsuccessful and return 0.0
        fprintf(stderr, "Memory allocation error\n");
        return 0.0;
    }

    // Tokenizes the sentence using whitespace as the delimiter
    char *token = strtok(processed_sentence, " ");
    // Iterate through each of the tokens retrieved
    while (token != NULL) {
        // Remove punctuation, but preserve special characters within the text file
        int j = 0;
        // Iterate through each character in the token
        for (int i = 0; token[i]; i++) {
            char c = token[i];
            // Preserve specific characters while removing punctuation
            if ( isalnum(c) || c == '\'' || c == '(' || c == ')' || c == '%' || c == '^' || c == '>' || c == '-' ||
                c == '<' || c == '/' || c == '*' || c == '@' || c == ':' || c == '{' || c == '}' ||
                c == '$' || c == '\\' || c == '#') {
                // Copy valid characters to the processed_sentence
                processed_sentence[j++] = c;
            }
        }
        // Null-terminate the processed_sentence
        processed_sentence[j] = '\0';

        // Convert token to lowercase
        for (int i = 0; processed_sentence[i]; i++) {
            processed_sentence[i] = tolower(processed_sentence[i]);
        }

        
        int found = 0;

        // Iterate and search for the token in the lexicon structure
        for (int i = 0; i < lexicon_dimension; i++) {
            // Compared the token and see if it matches with any word in the lexicon
            if (strcmp(processed_sentence, lexicon[i].word) == 0) {
                // Increment the score and word count for match
                total_score += lexicon[i].score;
                word_count++;
                found = 1;
                break;
            }
        }

        // Check if the token is not found in the lexicon
        if (!found) {
            // Assign a score of 0 for unknown words and increment count 
            total_score += 0;
            word_count++;
        }


        // Move to the next token
        token = strtok(NULL, " ");
    }


        // Free the memory allocated to processed_sentence
        free(processed_sentence);

        // Calculate Sentiment score and return it
        return word_count > 0 ? total_score / word_count : 0.0;
}

// Function to free memory allocated for the lexicon
void free_lexicon(struct words *lexicon, int lexicon_dimension) {
    // Loop through each word
    for (int i = 0; i < lexicon_dimension; i++) {
        // Free the memory for each word
        free(lexicon[i].word);
    }
    // Free the memory for the structure
    free(lexicon);
}