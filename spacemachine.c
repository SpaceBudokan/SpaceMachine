#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define INT uint16_t

INT programRam[65536];
INT *stackArray;
INT stackSize = 100;
INT *returnArray;
INT returnSize = 25;
INT programCounter = 0;
INT stackPointer = 0;
INT framePointer = 0;
INT returnPointer = 0;

/*Does nothing and does it well. */
void noOperation(void)
{
    programCounter++;
    return;
}

/*Pushes the value on the next cell onto the stack. */
void push(void)
{
    programCounter++;
    stackArray[stackPointer] = programRam[programCounter];
    stackPointer++;
    programCounter++;
    return;
}

/* "Drops" top of stack */
void pop(void)
{
    stackPointer--;
    programCounter++;
    return;
}

/* Sets the frame pointer for local variable use. Not required to call a subroutine. */
void newFrame(void)
{
    stackArray[stackPointer] = framePointer;
    framePointer = stackPointer;
    stackPointer++;
    programCounter++;
    return;
}

void jumpToSubroutine(void)
{   
    returnArray[returnPointer] = programCounter + 1;
    returnPointer++;
    stackPointer--;
    programCounter = stackArray[stackPointer];
    return;
}

void outputChar(void)
{
    stackPointer--;
    putchar((char)stackArray[stackPointer]);
    programCounter++;
    return;
}

/*Stops the whole thing and exits back to the operating system. */
void halt(void)
{
    free(stackArray);
    free(returnArray);
    exit(0);
}

void returnFromFunction(void)
{
    returnPointer--;
    programCounter = returnArray[returnPointer];
    return;
}

void jump(void)
{
    stackPointer--;
    programCounter = stackArray[stackPointer];
    return;
}

void duplicate(void)
{
    stackArray[stackPointer] = stackArray[stackPointer - 1];
    stackPointer++;
    programCounter++;
}

void ramLoad(void)
{

    stackArray[stackPointer - 1] = programRam[stackArray[stackPointer - 1]];
    programCounter++;
    return;
}

void ramStore(void)
{
    stackPointer--;
    programRam[stackArray[stackPointer]] = stackArray[stackPointer -1];
    stackPointer--;
    programCounter++;
    return;
}

void decode(void)
{
    /*I'm using a jumptable here because I wanted to see if I could make it work. A sane person would use a switch case loop and let the compiler decide. */
    void (*jumptable[12])(void) =
	{noOperation, push, pop, newFrame, jumpToSubroutine,
	outputChar, halt, returnFromFunction, jump, duplicate,
	ramLoad, ramStore};
  
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
    
    /*Test Program. Please ignore*/
    programRam[0] = 0;
    programRam[1] = 0;
    programRam[2] = 0;
    programRam[3] = 0;
    programRam[4] = 0;
    programRam[5] = 0;
    programRam[6] = 0;
    programRam[7] = 0;
    programRam[8] = 0;
    programRam[9] = 0;
    programRam[10] = 0;
    programRam[11] = 0;
    programRam[12] = 0;
    programRam[13] = 0;
    programRam[14] = 0;
    programRam[15] = 0;
    programRam[16] = 0;
    programRam[17] = 0;
    programRam[18] = 0;
    programRam[19] = 0;
    programRam[20] = 0;
    
    
    while(programCounter < 65535){
	decode();
    }
  
    free(stackArray);
    free(returnArray);
    return 0;
}
