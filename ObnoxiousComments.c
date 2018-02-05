/*
When working with single line comments,
the character \ acts as an escape character for '\n'.
The compiler will ignore the newline character and treat
the next line as a continuation of the commented out line.
*/

#include <stdio.h>

int main(void){
  printf("Before Obnoxious Comment\n");
  
  //This is an Obnoxious Comment \
  printf("THIS LINE WILL NEVER BE PRINTED :( \n");
  
  printf("After Obnoxious Comment\n");
  return 0;
}
