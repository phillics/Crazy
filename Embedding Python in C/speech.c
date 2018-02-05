#include "speech.h"

Speech* Speech_init(char author[], char speech[], long len){
    long i, k;
    int c, charUsed;
    char word[25] = {"\0"};
    int wordlen = 0;

    //create int array for following
    long counters[6] = {0};
    /*numberChars; numberSyls; numberWords; numberPoly; numberHard; numberSen;*/

    char sentenceTerminator[6] = {'.', '?', '!','\n',';', ':'};
    char wordTerminator[3] = {' ', '\t', ','};

    for(i = 0; i < len; i++){//go through speech char by char
        c = speech[i];
        charUsed = 0;

        //check for EOF (implies new sentance and new word)
        if(c == EOF){
            charUsed = 1;
            //new sentence
            counters[5]++;
            //new word
            newWord(counters, word, wordlen);
            //end for loop
            i = len+1;
        }

        //check for new sentence (implies new word)
        for(k = 0; k < 6; k++)
            if(c == sentenceTerminator[k]){
                charUsed = 1;
                //new sentence
                counters[5]++;
                //new word
                newWord(counters, word, wordlen);
                wordlen = 0;
                break;
            }

        //check for new word
        for(k = 0; k < 3; k++)
            if(c == wordTerminator[k]){
                charUsed = 1;
                newWord(counters, word, wordlen);
                wordlen = 0;
                break;
            }

        //non-word terminating characters
        if(c == '"' || c == '-')
            charUsed = 1;

        //if char not used, add to word and increment wordlen
        if(!charUsed){
            word[wordlen] = c;
            wordlen++;
        }
    }

    //create Speech struct
    Speech* self = malloc(sizeof(Speech));
    strcpy(self->author, author);
    self->numCharacters = counters[0]; //excluding whitespace
    self->numSyllables = counters[1];
    self->numWords = counters[2];
    self->numPolysyllable = counters[3]; //3+ syllable words
    self->numHardWords = counters[4]; //3+ syllable, excluding Nouns, hyphenated
    self->numSentences = counters[5];
    self->next = NULL;
    return self;
}

/*given word, find syllables and increment relevent counters:
number of characters, syllables, words, Polysyllable word?, Hard word?*/
void newWord(long counters[], char word[], int len){
    //get number of syllables
    int tempsyl;
    if(len > 0){
        tempsyl = numSyllables(word, len);

        if(tempsyl > 0){
            counters[0] += len;
            counters[1] += tempsyl;
            counters[2] += 1;

            if(tempsyl >= 3){
                if(!isupper(word[0]))
                    counters[4]++;
                counters[3]++;
                //printf("%s\t%d\n", word, tempsyl);
            }
        }
        //word troubleshooting
        //printf("%s\t\t%d\n", word, tempsyl);

        //reset word
        int a = 0;
        for(a = 0; a < len; a++)
            word[a] = '\0';
    }
}


/*given word, return number of syllables
each vowel in a word is considered one syllable
Exceptions:
    words of three letters or shorter count as single syllable
    consecutive vowels count as one syllable
    if y is at end or not next to vowel, counts as syllable*/
int numSyllables(char word[], int len){
    if(len < 3)
        return 1;

    char vowels[10] = {'a','e','i','o','u','A','E','I','O','U'};
    int i, v, prevVowel = 0, isVowel = 0, syl = 0;

    for(i = 0; i <= len; i++){
        isVowel = 0;

        for(v = 0; v < 10; v++) //for each letter, check against each vowel
            if(word[i] == vowels[v]){ //if current is vowel
                if(!prevVowel){ //if previous wasn't vowel
                    syl++;
                    v = 1000;
                }
                isVowel = 1;
            } else { //current is not vowel

            }

        if(isVowel)
            prevVowel = 1;
        else
            prevVowel = 0;

        if(word[i] == 'y')
            if(!prevVowel){
                syl++;
                prevVowel = 1;
            }
    }

    return syl;
}

void Speech_display(Speech* spc){
    printf("AUTHOR      \t%s\n", spc->author);
    printf("Characters  \t%ld\n", spc->numCharacters);
    printf("Syllables   \t%ld\n", spc->numSyllables);
    printf("Words       \t%ld\n", spc->numWords);
    printf("Polysyllable\t%ld\n", spc->numPolysyllable);
    printf("Hard Words  \t%ld\n", spc->numHardWords);
    printf("Sentences   \t%ld\n", spc->numSentences);
}

void Speech_destroy(Speech* spc){
    free(spc);
}
