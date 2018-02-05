#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "readability.h"

int main(){
    char text[] = {"The rule of rhythm in prose is not so intricate. Here, too, we write in "
        "groups, or phrases, as I prefer to call them, for the prose phrase is greatly "
        "longer and is much more nonchalantly uttered than the group in verse; so that not "
        "only is there a greater interval of continuous sound between the pauses, but, for "
        "that very reason, word is linked more readily to word by a more summary enunciation. "
        "Still, the phrase is the strict analogue of the group, and successive phrases, like "
        "successive groups, must differ openly in length and rhythm. The rule of scansion in "
        "verse is to suggest no measure but the one in hand; in prose, to suggest no measure "
        "at all. Prose must be rhythmical, and it may be as much so as you will; but it must "
        "not be metrical. It may be anything, but it must not be verse."};

    char text2[] = {"The best things in an artist’s work are so much a matter of intuition, "
        "that there is much to be said for the point of view that would altogether discourage "
        "intellectual inquiry into artistic phenomena on the part of the artist. Intuitions are shy "
        "things and apt to disappear if looked into too closely. And there is undoubtedly a danger "
        "that too much knowledge and training may supplant the natural intuitive feeling of a student, "
        "leaving only a cold knowledge of the means of expression in its place. For the artist, if he has "
        "the right stuff in him, can accomplish anything."};

    long length = (long) (sizeof(text) / sizeof(char));
    long length2 = (long) (sizeof(text2)/ sizeof(char));
    /*text 1
    words: 151
    sentences: 6
    characters: 623
    syllables: 206

    text 2
    words: 100
    characters: 448
    sentences: 4
    */

    Speech* exSpeech = Speech_init("Christian", text, length);

    Speech_display(exSpeech);
    printf("\n\n");
    printf("Automated    \t%.1lf\n", readability_Automated(exSpeech));
    printf("ColemanLiau  \t%.1lf\n", readability_ColemanLiau(exSpeech));
    printf("Flesch       \t%.1lf\n", readability_Flesch(exSpeech));
    printf("FleschKincaid\t%.1lf\n", readability_FleschKincaid(exSpeech));
    printf("FOG          \t%.1lf\n", readability_FOG(exSpeech));
    printf("Linsear      \t%.1lf\n", readability_Linsear(exSpeech));
    printf("SMOG         \t%.1lf\n", readability_SMOG(exSpeech));
    return 0;

}
