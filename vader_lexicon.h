#ifndef VADER_LEXICON_H
#define VADER_LEXICON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct words {
    char *word;
    float score;
    float SD;
    int SIS_array[10];
};

int read_vader_file(const char *filename, struct words **lexicon, int *lexicon_size);
float calculate_sentiment_score(const char *sentence, struct words *lexicon, int lexicon_size);
void free_lexicon(struct words *lexicon, int lexicon_size);

#endif /* VADER_LEXICON_H */