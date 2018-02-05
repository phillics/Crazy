/*
President structure stores each speech
President.c contains functions that calculate a president's

*/
#ifndef PRESIDENT_H
#define PRESIDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "speech.h"
#include "readability.h"


//president struct is a linked list
typedef struct President{
    char name[25]; //longest name is 22 characters
    int numberOfSpeeches;

    double scoreAuto;
    double scoreColeman;
    double scoreFlesch;
    double scoreFleschK;
    double scoreFOG;
    double scoreLinsear;
    double scoreSMOG;

    Speech* start; //first speech
    Speech* end; //last speech
}President;

President* President_init(Speech* spc);

void President_add(President* pres, Speech* spc);

void President_remove(President* pres, Speech* spc);

//find average score for Auto - SMOG
void President_findAverages(President* pres);

void President_display(President* pres);

void President_displaySpeeches(President* pres);

void President_destroy(President* pres);

#endif//PRESIDENT_H
