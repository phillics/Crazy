/*calculating words...
periods, question marks, explanation points, colons, and semi colons are sentence delimiters
each group of continuous non-blank characters counts as a word,

    data to be collected during read:
number of characters (excluding whitespace)
number of syllables
number of words
number of polysyllable words (3+ syllables)
number of difficult words (polysyllable excluding proper nouns)
number of sentences
*/
#ifndef SPEECH_H
#define SPEECH_H

#include <stdio.h>
#include <string.h>//string manipulations
#include <stdlib.h>//dynamic memory
#include <ctype.h> //isupper()

typedef struct Speech{
    char author[25]; //longest name is 22 characters

    long numCharacters; //excluding whitespace
    long numSyllables;
    long numWords;
    long numPolysyllable; //3+ syllable words
    long numHardWords; //3+ syllable, excluding Nouns, not hyphenated
    long numSentences;

    //for use in President list
    struct Speech* next;
} Speech;

//Speech functions
Speech* Speech_init(char author[], char speech[], long len);
void Speech_display(Speech *spc);
void Speech_destroy(Speech *spc);

//sub functions for Speech_init
void newWord(long[], char[], int);
int numSyllables(char word[], int len);


#endif//SPEECH_H
