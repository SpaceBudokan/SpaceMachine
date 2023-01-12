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
INT returnPointer = 0;

/*Does nothing and does it well. */
void noOperation(void)
{
    return;
}

/*Pushes the value on the next cell onto the stack. */
void push(void)
{
    programCounter++;
    stackArray[stackPointer] = programRam[programCounter];
    stackPointer++;
    return;
}
    
/*Stops the whole thing and exits back to the operating system. */
void halt(void)
{
    free(stackArray);
    free(returnArray);
    exit(0);
}

void decode(void)
{
    /*using a jumptable because some compilers aren't too eager to optimize switch-case loops to jumptablesI'm using a jumptable here because I wanted to see if I could make it work. A sane person would use a switch case loop and let the compiler decide. */
    void (*jumptable[3])(void) = {noOperation, push, halt};
  
    jumptable[programRam[programCounter]]();
    return;
}

int main(int argc, char **argv)
{
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
    
    while(programCounter < 100){
	decode();
	programCounter++;
    }
  
    free(stackArray);
    free(returnArray);
    return 0;
}
