#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ncurses.h>

#define INT uint16_t

INT programRam[65536];
INT *stackArray;
INT stackSize = 100;
INT *returnArray;
INT returnSize = 100;
INT programCounter = 0;
INT stackPointer = 0;
INT framePointer = 0;
INT returnPointer = 0;


/* Sets up the pointers. */
void initialize(void)
{
    stackArray = malloc(sizeof(INT) * stackSize);
    if(stackArray == NULL){
	printf("ERROR: Unable to allocate stackArray!\n");
	exit(1);
    }

    returnArray = malloc(sizeof(INT) * returnSize);
    if(returnArray == NULL){
	printf("ERROR: Unable to allocate returnArray!\n");
	free(stackArray);
	exit(1);
    }

    /*ncurses setup */
    initscr();
    cbreak();
    noecho();
    /*nonl();*/ /* Turn off newline when enter is hit.*/
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    return;    
}

/* frees the pointers and stops ncurses */
void shutdown(void)
{
    free(stackArray);
    free(returnArray);
    endwin();
    return;
}
    
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

/* Sets the frame pointer for local variable use. Not required to call a subroutine. You probably wouldn't even bother with this if you're programming in assembly.*/
void newFrame(void)
{
    stackArray[stackPointer] = framePointer;
    framePointer = stackPointer;
    stackPointer++;
    programCounter++;
    return;
}

/* increments the program counter and pushes it to the return stack, then pops the stop of stack and jumps to that adress.*/
void jumpToSubroutine(void)
{   
    returnArray[returnPointer] = programCounter + 1;
    returnPointer++;
    stackPointer--;
    programCounter = stackArray[stackPointer];
    return;
}

/* Pops the top of the stack and sends it to stdscr as a character. */
void outputScr(void)
{
    stackPointer--;
    /* putchar((char)stackArray[stackPointer]);*/
    addch((char)stackArray[stackPointer]);
    programCounter++;
    return;
}

/*Stops the whole thing and exits back to the operating system. */
void halt(void)
{
    free(stackArray);
    free(returnArray);
    endwin();
    exit(0);
}

/* pops the top of the return stack and moves the program counter to that address. */
void returnFromFunction(void)
{
    returnPointer--;
    programCounter = returnArray[returnPointer];
    return;
}

/*  Pops the top of the stack and set the program counter to that address. */
void jump(void)
{
    stackPointer--;
    programCounter = stackArray[stackPointer];
    return;
}

/* Duplicates the top of the stack. */
void duplicate(void)
{
    stackArray[stackPointer] = stackArray[stackPointer - 1];
    stackPointer++;
    programCounter++;
}

/* Pops the top of the stack, and puts the value at that adress to the top of the stack. */
void ramLoad(void)
{

    stackArray[stackPointer - 1] = programRam[stackArray[stackPointer - 1]];
    programCounter++;
    return;
}

/* Pops the top of the stack and stores the value on the top of the stack at that address.*/
void ramStore(void)
{
    stackPointer--;
    programRam[stackArray[stackPointer]] = stackArray[stackPointer -1];
    stackPointer--;
    programCounter++;
    return;
}

/* get's a charcter from the keyboard and placed it on the top of the stack. Does NOT echo. */
void getCharacter(void)
{
    char character;
    character = getch();
    stackArray[stackPointer] = (INT)character;
    stackPointer++;
    programCounter++;
    return;
}

/* jumps to value in next cell if zero, otherwise it skips it */
void jumpIfFalse(void)
{
    stackPointer--;
    programCounter++;
    if(stackArray[stackPointer] == 0){
	programCounter = programRam[programCounter];
    } else{
	programCounter++;
    }
    
    return;
}

/* copies the value under the top of the stack to the top of the stack */
void over(void)
{
    stackArray[stackPointer] = stackArray[stackPointer - 2];
    stackPointer++;
    programCounter++;
    return;
}


/* Moves the frame pointer back to it's previous position. Does not pop anything.*/
void killFrame(void)
{
    framePointer = stackArray[framePointer];
    programCounter++;
    return;
}

/* Get's a value offset relative to the frame pointer (taken from the stack)  and copies it to the top of the stack. */
void loadLocal(void)
{
    INT offset;
    offset = stackArray[stackPointer -1];
    stackArray[stackPointer -1] = stackArray[framePointer - offset];
    programCounter++;
    return;
}

/* Stores a value on the stack offset relative to the frame pointer. It's argument is on the top of the stack. The value to store is beneath that. */
void storeLocal(void)
{
    INT offset;
    INT value;
    offset = stackArray[stackPointer - 1];
    value = stackArray[stackPointer - 2];
    stackArray[framePointer - offset] = value;
    stackPointer -= 2;
    programCounter++;
}

/* Moves the current top of the stack to the return stack */
void toReturn(void)
{
    stackPointer--;
    returnArray[returnPointer] = stackArray[stackPointer];
    returnPointer++;
    programCounter++;
    return;
}

/* Moves current top of the return stack to the data stack. */
void fromReturn(void)
{  
    returnPointer--;
    stackArray[stackPointer] = returnArray[returnPointer];
    stackPointer++;
    programCounter++;
    return;
}

/* adds the top two elements of the stack */
void addition(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer - 1] + stackArray[stackPointer];
    programCounter++;
    return;
}

/* Subtracts the top of the stack from the next value on the stack. Think of the stack as growing from left to right, and you stick the minus in betwen the top two elements. If the stack is DCBA the subtraction operation does B-A */
void subtraction(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] - stackArray[stackPointer];
    programCounter++;
    return;
}

/* remember there are unsigned integers */
void multiplication(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] * stackArray[stackPointer];
    programCounter++;
    return;
}

/* Like subtraction, if the stack is DCBA, this returns B/A */
void division(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] / stackArray[stackPointer];
    programCounter++;
    return;
}

/* returns B%A */
void modulus(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] % stackArray[stackPointer];
    programCounter++;
    return;
}

/* Zero is false. Nonzero is true. */
void and(void)
{
    stackPointer--;
    if((stackArray[stackPointer -1] >= 1) && (stackArray[stackPointer] >= 1)){
	stackArray[stackPointer - 1] = 1;
    } else {
	stackArray[stackPointer - 1] = 0;
    }
    programCounter++;
    return;
}

/* Zero is false. Nonzero is true. */
void or(void)
{
    stackPointer--;
    if((stackArray[stackPointer -1] >= 1) || (stackArray[stackPointer] >= 1)){
	stackArray[stackPointer - 1] = 1;
    } else {
	stackArray[stackPointer - 1] = 0;
    }
    programCounter++;
    return;
}


/* Zero is false. Nonzero is true. */
void not(void)
{
    stackPointer--;
    if(stackArray[stackPointer -1] >= 1){
	stackArray[stackPointer - 1] = 0;
    } else {
	stackArray[stackPointer - 1] = 1;
    }
    programCounter++;
    return;
}


void bitwiseAnd(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] & stackArray[stackPointer];
    programCounter++;
    return;
}

void bitwiseOr(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] | stackArray[stackPointer];
    programCounter++;
    return;
}

void bitwiseXor(void)
{
    stackPointer--;
    stackArray[stackPointer - 1] = stackArray[stackPointer -1] ^ stackArray[stackPointer];
    programCounter++;
    return;
}

void equal(void)
{
    stackPointer--;
    if(stackArray[stackPointer -1] == stackArray[stackPointer]){
	stackArray[stackPointer - 1] = 1;
    } else {
	stackArray[stackPointer - 1] = 0;
    }
    programCounter++;
    return;
}


/* Pops the top of the stack and sends it to stdout as a character. */
void outputStdout(void)
{
    stackPointer--;
    putchar((char)stackArray[stackPointer]);
    programCounter++;
    return;
}



void decode(void)
{
    /*I'm using a jumptable here because I wanted to see if I could make it work. A sane person would use a switch case loop and let the compiler decide. */
    void (*jumptable[33])(void) =
	{noOperation, push, pop, newFrame, jumpToSubroutine,
	 outputScr, halt, returnFromFunction, jump, duplicate,
	 ramLoad, ramStore, getCharacter, jumpIfFalse, over,
	 killFrame, loadLocal, storeLocal, toReturn, fromReturn,
	 addition, subtraction, multiplication, division, modulus,
	 and, or, not, bitwiseAnd, bitwiseOr,
	 bitwiseXor, equal, outputStdout};
  
    jumptable[programRam[programCounter]]();
    return;
}



int main(int argc, char **argv){

    initialize();
   
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
    programRam[20] = 6;
    
    
    while(programCounter < 65535){
	decode();
    }
  
    shutdown();
    return 0;
}
