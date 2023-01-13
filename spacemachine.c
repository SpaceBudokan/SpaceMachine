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

void decode(void)
{
    /*I'm using a jumptable here because I wanted to see if I could make it work. A sane person would use a switch case loop and let the compiler decide. */
    void (*jumptable[7])(void) =
	{noOperation, push, pop, newFrame, jumpToSubroutine,
	outputChar, halt};
  
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
    programRam[0] = 1;
    programRam[1] = 5;
    programRam[2] = 3;
    programRam[3] = 1;
    programRam[4] = 10;
    programRam[5] = 4;
    programRam[6] = 1;
    programRam[7] = 98;
    programRam[8] = 6;
    programRam[9] = 0;
    programRam[10] = 1;
    programRam[11] = 97;
    programRam[12] = 5;
    programRam[13] = 6;
    programRam[14] = 0;
    programRam[15] = 0;
    programRam[16] = 0;
    programRam[17] = 0;
    
    
    while(programCounter < 65535){
	decode();
    }
  
    free(stackArray);
    free(returnArray);
    return 0;
}
