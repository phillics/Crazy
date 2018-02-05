#include "readability.h"

/*AutomatedReadability = 4.71 x (characters/words) + .5 x (words/sentences) - 21.43
outputs US Grade level,
is fairly accurate for all texts
*/
double readability_Automated(Speech* spc){
    double charPerWord = ((double) spc->numCharacters);
    if(spc->numWords > 1)
        charPerWord =  charPerWord / ((double) spc->numWords);

    double wordsPerSen = ((double) spc->numWords);
    if(spc->numSentences > 1)
        wordsPerSen = wordsPerSen / ((double) spc->numSentences);

    return 4.71*charPerWord + .5*wordsPerSen - 21.43;
}

/*SmogIndex - Grade Level = 3+sqrt( (PSC / Numsentences) x 30 )
        PSC = PolysyllableCount: count every word with 3+ syllables
outputs US Grade level, most accurate for grades 4-16
used for conusmer-oriented healthcare material
*/
double readability_SMOG(Speech* spc){
    double gradeLevel = 30.0 * ((double) spc->numPolysyllable);
    if(spc->numSentences > 1)
        gradeLevel = gradeLevel / ((double) spc->numSentences);

    return sqrt(gradeLevel);
}

/*Flesch reading ease  - RE = 206.835 - (1.015 x ASL) - (84.6 x ASW)
        RE = Readability Ease, ASL = Average Sentence Length, ALW average syllables per word
        works best with more advanced writing (college level)
    0-100, higher score is easier reading:
    90-100 - 5th Grade
    60-70 - pre-highschool
    50-60 - early highschool 9,10
    40-50 - late high school 11,12
    30-40 - college level    13-16
    0-30 - college graduate  17+    */
double readability_Flesch(Speech* spc){
    double asl = ((double) spc->numWords);
    if(spc->numSentences > 1)
        asl = asl / ((double) spc->numSentences);

    double asw = ((double) spc->numSyllables);
    if(spc->numWords > 1)
        asw = asw / ((double) spc->numWords);

    return 206.835 - (1.015 * asl) - (84.6 * asw);
}

/*Flesch-Kincaid - FKRA = (0.39 x ASL) + (11.8 x ASW) - 15.59
    outputs US Grade level (best if bewteen 3 and 12) 12+ is very inaccurate*/
double readability_FleschKincaid(Speech* spc){
    double asl = ((double)spc->numWords);
    if(spc->numSentences > 1)
        asl = asl / ((double)spc->numSentences);

    double asw = ((double)spc->numSyllables);
    if(spc->numWords > 1)
        asw = asw / ((double)spc->numWords);

    return (0.39 * asl) + (11.8 * asw) - 15.59;
}

/*FogIndex - Grade Level = .04(ASL + PHW)
    PHW is percentage of hard words
        hard words are 3+ syllables and NOT: proper nouns, hyphenated, 3syl end with -es or -ed
    5 is easy, 10 is medium, 15 is difficult, 20 is incredibly difficult */
double readability_FOG(Speech* spc){
    double asl = ((double)spc->numWords);
    if(spc->numSentences > 1)
        asl = asl / ((double)spc->numSentences);

    double phw = 100 * ((double)spc->numHardWords);
    if(spc->numHardWords > 1)
        phw = phw / ((double)spc->numWords);

    return 0.4 * (asl + phw);
}

/*Coleman-Liau - CLI = 0.0588 x (letters per 100 words) - 0.296 x (sentences per 100 words) - 15.8
    outputs US Grade level*/
double readability_ColemanLiau(Speech* spc){
    double lettersPerHWord = 100.0 * ((double) spc->numCharacters);
    double sentencesPerHWord = 100.0 * ((double) spc->numSentences);
    if(spc->numWords > 1){
        lettersPerHWord  = lettersPerHWord / ((double) spc->numWords);
        sentencesPerHWord = sentencesPerHWord / ((double) spc->numWords);
    }

    return 0.0588 * lettersPerHWord - 0.296 * sentencesPerHWord - 15.8;
}

/*LinsearWrite  avg = 100 * (Easy*1 + Hard*3) / (numWords)
    avg /= numSentences
    if ans > 20, ans /= 2
    else ans = (ans - 2) / 2;*/
double readability_Linsear(Speech *spc){
    double hard = (double) spc->numHardWords;
    double total = (double) spc->numWords;
    double easy =  total - hard;

    if(total < 1)
        return 0.0;

    double linsear = (easy + hard * 3) / total;

    linsear = (linsear-1.0) * 100.0;

    if(linsear > 20)
        return linsear / 2;
    return (linsear - 2.0)/2;
}
