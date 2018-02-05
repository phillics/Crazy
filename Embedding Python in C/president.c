#include "president.h"

President* President_init(Speech* spc){
    President* self = malloc(sizeof(President));

    //add spc as first element of list
    self->start = spc;
    self->end = spc;

    strcpy(self->name, spc->author);
    self->numberOfSpeeches = 1;

    //initialize scores to -1 to show they have not been calculated yet.
    //NOTE: may need to rethink this as -1 is a possible score on some of these tests...
    self->scoreAuto = -1;
    self->scoreColeman = -1;
    self->scoreFlesch = -1;
    self->scoreFleschK = -1;
    self->scoreFOG = -1;
    self->scoreLinsear = -1;
    self->scoreSMOG = -1;

    return self;
}

void President_add(President* pres, Speech* spc){
    //add to end
    pres->end->next = spc;
    pres->end = spc;
    pres->numberOfSpeeches += 1;
}

void President_display(President* pres){
    //display president stuff
    printf("%s\n", pres->name);
    printf("Number of Speeches: %d\n\n", pres->numberOfSpeeches);
}

void President_displaySpeeches(President* pres){
    Speech* cur = pres->start;
    int count = 0;
    while(cur != NULL){
        printf("SPEECH %d", ++count);
        Speech_display(cur);
        cur = cur->next;
    }

}

//find average score for every speech given by president
void President_findAverages(President* pres){
    double sums[7] = {0.0};
    Speech* cur = pres->start;
    double count = 0.0;

    while(cur != NULL){
        //pass cur to each function to determine score
        sums[0] += readability_Automated(cur);
        sums[1] += readability_ColemanLiau(cur);
        sums[2] += readability_Flesch(cur);
        sums[3] += readability_FleschKincaid(cur);
        sums[4] += readability_FOG(cur);
        sums[5] += readability_Linsear(cur);
        sums[6] += readability_SMOG(cur);

        count += 1.0;
        cur = cur->next;
    }
    //average = sum / count
    pres->scoreAuto = sums[0] / count;
    pres->scoreColeman = sums[1] / count;
    pres->scoreFlesch = sums[2] / count;
    pres->scoreFleschK = sums[3] / count;
    pres->scoreFOG = sums[4] / count;
    pres->scoreLinsear = sums[5] / count;
    pres->scoreSMOG = sums[6] / count;
}

void President_remove(President* pres, Speech* spc){
    //do not destroy speech
    Speech* cur = pres->start;
    Speech* prev = NULL;
    while(cur != NULL){
        if(cur == spc){
            if(prev == NULL){ //remove first element
                pres->start = cur->next;
                return;
            } else {
                prev->next = cur->next;
                return;
            }
        }
        prev = cur;
        cur = cur->next;
    }
    printf("ERROR: SPEECH NOT FOUND\n");
}

void President_destroy(President* pres){
    //do not destroy speeches
    free(pres);
}
