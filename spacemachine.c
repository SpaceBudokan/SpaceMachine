#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define INT uint16_t

INT programRam[100]; /* Temporary for testing. The finished program will load the prpogram from a file. */
INT *stackArray;
INT stackSize = 100;
INT *returnArray;
INT returnSize = 25;
INT programCounter = 0;
INT stackPointer = 0;


void decode(void)
{
  /*using a jumptable because some compilers aren't too eager to optimize switch-case loops to jumptablesI'm using a jumptable here because I wanted to see if I could make it work. A sane person would use a switch case loop and let the compiler decide. */
  void (*jumptable[1])(void) = {halt};
  
  jumptable[programRam[pc]]();
  return;
}

int main(int argc, char **argv)
{
  int i = 0;
  stackArray = malloc(sizeof(INT) * stackSize);
  if(stackArray == NULL){
    printf("ERROR: Unable to allocate stackArray!\n");
    return 1;
  }

  returnArray = malloc(sizeof(INT) * returnSize);
  if(returnArray == NULL){
    printf("ERROR: Unable to allocate returnArray!\n");
    return 1;
  }

    
  while(programCounter < programLength){
    decode();
    programCounter++;
    }
  
    free(stackArray);
    free(returnArray);
    return 0;
}
