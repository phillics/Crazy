/*contains various readability formulas
http://www.readabilityformulas.com/free-readability-formula-tests.php

Colemanliau - used for public education textbooks
Flesch - used by government agencies (Department of Defense)
Flesch-Kincaid - default in MS-Word: used by education systems and DOD
FOG - newspapers and magazines
Linsear - Air Force training manuals 
*/
#ifndef READABILITY_H
#define READABILITY_H

//standard files
#include <stdio.h>
#include <math.h>

//my files
#include "speech.h"


//function prototypes
double readability_Automated(Speech* spc); //       OK
double readability_ColemanLiau(Speech* spc);//+1.3  OK
double readability_SMOG(Speech* spc);//+.7          GOOD
double readability_Flesch(Speech* spc);//-16        REALLY BAD
double readability_FleschKincaid(Speech* spc);//+2  OK
double readability_FOG(Speech* spc); //+.8          OK
double readability_Linsear(Speech *spc); //NEEDS SERIOUS HELP


#endif//READABILITY_H
