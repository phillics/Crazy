/*
Christian Phillips 9/27/16
compile with:
    gcc -Wall test.c readability.c speech.c president.c -I/usr/include/python2.7 -lpython2.7 -lm
purpose:
    download presendential speeches using python webbot and save in folder
    analyze each speech using readability analysis.
source material:
    https://docs.python.org/2.7/extending/extending.html#a-simple-example
*/

//STANDARD LIBRARY FILES///////////////////////////////////////////////////////
#include <Python.h> //must include python.h before other headers
#include <stdio.h>
#include <string.h>

//MY FILES/////////////////////////////////////////////////////////////////////
#include "readability.h"
#include "speech.h"
#include "president.h"
//#include "stats.h" //unfinished...


#ifdef linux
    #define PAUSE   printf("\n\nPress any key to continue... ");getchar();
    #define FLUSH   while(getchar() != '\n'){;}
    #define CLS     system("clear");
#else
    #define PAUSE   system("pause");
    #define FLUSH   system("flush");
    #define CLS     system("cls");
#endif//LINUX OS check


/*/////////////////////////////////////////////////////////////////////////////
FUNCTION PROTOTYPES
*//////////////////////////////////////////////////////////////////////////////
void runWebBot();

//INITIALIZING STRUCTURES
//add each speech to corresponding president
void populatePresidents(President* presidents[], int* presctr, Speech* speeches[], int speechCtr);
//read text files into string, store string data in speech struct
void populateSpeeches(Speech* speeches[], int *speechCtr);

//ANALYSIS
//display contents of array
void displayHistogram(President* presidents[], int data[], int num);
//organizes input for displayHistogram
void displayStats(President* presidents[], int presidentCtr);

//GENERAL FILE READING
//reads each line of file into given char array, returns length
int readFileLine(FILE *fp, char line[], int lim);
//opens file, reads contents to string, returns pointer to string
char* readFileToString(char fileName[], int nameLen, long *fileLength);
//returns pointer to author's name
char* getAuthorFromTitle(char fileName[], int titleLen);




int main()//int argc, char *argv[])
{
    /*/////////////////////////////////////////////////////////////////////////
    RUN WEBBOT TO DOWNLOAD FILES
    *//////////////////////////////////////////////////////////////////////////
    runWebBot();
    PAUSE;


    /*/////////////////////////////////////////////////////////////////////////
    OPEN FILES AND CREATE SPEECH STRUCTS
    *//////////////////////////////////////////////////////////////////////////
    //array of Speech pointers
    Speech* speeches[1000];//there are 979 speeches
    int speechCtr = 0; //next open position

    //read Speeches int Speech structs
    populateSpeeches(speeches, &speechCtr);


    /*/////////////////////////////////////////////////////////////////////////
    CREATE ARRAY OF PRESIDENT STRUCTS
    *//////////////////////////////////////////////////////////////////////////
    //array of President pointers
    President* presidents[45]; //43 presidents
    int presidentCtr = 0; //next open position

    //organize speeches into president structures as elements of a linked list
    populatePresidents(presidents, &presidentCtr, speeches, speechCtr);

    //exit program if error populating Presidents...
    if(presidentCtr == 0){
        printf("Error Populating\n");
        return 0;
    }

    //for each president, find average score using each formula
    int i;
    for(i = 0; i <= presidentCtr; i++)
        President_findAverages(presidents[i]);


    /*/////////////////////////////////////////////////////////////////////////
    ANALYSIS
    *//////////////////////////////////////////////////////////////////////////
    displayStats(presidents, presidentCtr);

    //series of vertical IQR box plots...???
    //write to file: sorted table of all words in all files


    /*/////////////////////////////////////////////////////////////////////////
    CLEAN-UP    free all structures
    *//////////////////////////////////////////////////////////////////////////
    for(i = 0; i < presidentCtr; i++)
        President_destroy(presidents[i]);

    for(i = 0; i < speechCtr; i++)
        Speech_destroy(speeches[i]);

    return 0;
}

void displayStats(President* presidents[], int presidentCtr){
    //create histogram array;
    int histArray[presidentCtr+1], i;

    //display histogram of number of speeches each president gave...
    printf("\n\n\t\t\t  NUMBER OF SPEECHES\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->numberOfSpeeches;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    //display histogram of scores vs president, for each test
    printf("\t\t\t  AUTOMATED READABILITY SCORE: (higher is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreAuto;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    printf("\t\t\t  COLEMAN-LIAU SCORE: (higher is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreColeman;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    printf("\t\t\t  FLESCH SCORE: (lower is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreFlesch;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    printf("\t\t\t  FLESCH-KINCAID SCORE: (higher is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreFleschK;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    printf("\t\t\t  FOG INDEX SCORE: (higher is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreFOG;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    printf("\t\t\t  LINSEAR SCORE: (higher is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreLinsear;
    displayHistogram(presidents, histArray, presidentCtr);
    PAUSE;    CLS;

    printf("\t\t\t  SMOG INDEX SCORE: (higher is harder)\n\n");
    for(i = 0; i <= presidentCtr; i++)
        histArray[i] = presidents[i]->scoreSMOG;
    displayHistogram(presidents, histArray, presidentCtr);
}

//take names from presidents, and display data in data
void displayHistogram(President* presidents[], int data[], int num){
    int i, k;

    for(i = 0; i <= num; i++){
        for(k = 0; k < 25; k++){
            if(presidents[i]->name[k] == '\0')
                break;
        }
        printf("%s", presidents[i]->name);

        for(k; k <= 25; k++)
            printf(" ");
        printf("|");

        for(k = 0; k <= data[i]; k++)
            printf("*");
        printf("\n");
    }
}


/*Given array of speeches, add each speech to corresponding president
after all speeches have been added (speech ptr = NULL), calculate average scores for each president
*/
void populatePresidents(President* presidents[], int *presidentCtr, Speech* speeches[], int speechCtr){
    int i = 0;
    int ctr = *presidentCtr;
    //causes seg fault
    while(speeches[i] != NULL && i < speechCtr){
        if(presidents[ctr] == NULL){ //if current president is empty, create president
            presidents[ctr] = President_init(speeches[i]);
        } else { //check if speech belongs to current president
            if( strcmp(speeches[i]->author, presidents[ctr]->name) == 0){
                President_add(presidents[ctr], speeches[i]);
            }else{
                ctr++;//sometimes causes seg fault...
            }
        }
        i++;
    }
    *presidentCtr = ctr;
    printf("EXITING POPULATE\n");
}

/*open directory file, read list of file names
for each file name:
    open corresponding file, get author and transcropt,
    create Speech struct
    populate Speech array
*/
void populateSpeeches(Speech* speeches[], int *speechCtr){
    //access directory file
    int lineLength;

    FILE *dirFile;
    dirFile = fopen("FILELIST.txt", "r");
    if(dirFile == NULL){
        printf("\n\nERROR: Could not open FILELIST.txt\n");
        return;
    }

    char fname[155] = {"\0"};//155 is max filename length

    char* author;
    char* transcript;

    //read FILELIST into Array;
    while((lineLength = readFileLine(dirFile, fname, 255)) > 20)
    {
        //get author from line

        author = getAuthorFromTitle(fname, 25); //25 is namelength

        //get transcript from file
        long fileLength = 0;
        transcript = readFileToString(fname, lineLength, &fileLength);

        //check author and transcript to make sure valid
        if(sizeof(author) < 4 || fileLength < 1){
            printf("Error:  File %d could not be read\n", *speechCtr);
            continue;
        }

        //create speech and add to speech list
        speeches[*speechCtr] = Speech_init(author, transcript, fileLength);
        //Speech_display(speeches[*speechCtr]);
        //printf("%s\n", fname);
        //printf("FLESCH:    %.1lf\n", readability_Flesch(speeches[*speechCtr]));

        *speechCtr += 1;

        //do I need to FREE transcript and author?
        free(author);
        free(transcript);
    }

    fclose(dirFile);
}

//reads to EOF, returns string ptr
char* readFileToString(char fileName[], int nameLen, long *length){

    char *transcript;
    long fileSize;

    int i;
    char fullFileName[200] = {'\0'};
    strcpy(fullFileName, "PresidentSpeeches/");

    for(i = 18; i < nameLen + 18; i++){
        fullFileName[i] = fileName[i-18];
    }

    //"Barack Obama Acceptance Speech at the Democratic National Convention (August 28, 2008).txt"
    FILE *filePtr = fopen(fullFileName, "rb");

    if(filePtr == NULL){
        printf("FILE COULD NOT BE OPENED\t");
        transcript = "FILE NOT FOUND";
        return transcript;//file could not be opened
    }

    fseek(filePtr, 0, SEEK_END);
    fileSize = ftell(filePtr);
    rewind(filePtr);

    //allocate space for string
    *length = fileSize;

    transcript = malloc((fileSize + 1) * (sizeof(char)));
    fread(transcript, sizeof(char), fileSize, filePtr);
    fclose(filePtr);
    transcript[fileSize] = 0;

    return transcript;
}

//used for FILELIST.txt, each line is a file name that is collected for future use
int readFileLine(FILE *fp, char line[], int lim){//read line, return length
    int c, i;
    //EOF is [ctrl] d
    for(i = 0; i < lim-1 && (c = fgetc(fp))!=EOF && c!='\n'; ++i)
        line[i] = c;

    line[i] = '\0';
    return i;
}

//read in two full words:  George H. W. Bush is 2 words... H. W. doesn't count...
char* getAuthorFromTitle(char fileName[], int titleLen){//returns pointer to author's name
    char *author;
    author = malloc((titleLen) * sizeof(char));

    int words = 0, i;
    for(i = 0; i < titleLen; i++)
        author[i] = '\0';

    i = 0;
    char c = '\0';
    while(words < 2 && i < (titleLen -1)){ //titleLen -1 to save room for '\0'
        c = fileName[i];

        if(c == '.')
            words--;
        if(c == ' ')
            words++;

        if(words >= 2)
            break;

        author[i] = fileName[i];
        i++;
    }

    return author;
}

/*runs python webbot: easier to write in python than in c
downloads speeches from online site, and saves each speech in seperate file
runtime is approx 12-15 min*/
void runWebBot(){//int argc, char *argv[]){
    char res = 'n';
    printf("Python Bot about to be launched. This will overwrite any saved data in "
        "PresidentSpeeches and will take about 15 minutes.\nContinue Launch?(y/n): ");
    scanf("%c", &res);
    FLUSH; //read in all remaining characters before python crashes...
    //if buffer is not flushed before python tries to get input, it will cause an error

    if(res != 'y')
        return;



    /*Inform python interpreter about paths to python run-time libraries:
    optional but recommended. Needs to be run before py_initialize*/
    //Py_SetProgramName(argv[0]);

    /*initialize interpreter*/
    Py_Initialize();

    /*runs given string directly on interpreter*/
    PyRun_SimpleString("from time import time,ctime\n"
                       "print 'Today is',ctime(time())\n");

    /*to run a python file, pass filepointer and file name*/
    FILE *fp;
    fp = fopen("webbot.py", "r");
    PyRun_SimpleFile(fp, "webbot.py");

    /*close interpreter*/
    Py_Finalize();
}
