#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
 *  ***** YOUR CODE WILL BE INSERTED HERE ******
 */
typedef enum {RED, YELLOW, GREEM, CYAN, BLUE, MAGENTA} Colour;
char* colours[] = {"Red","Yellow","Green","Cyan","Blue", "Magenta"};
/* 
 * Return true iff testNum is in the n-element array 'array'
 * ["iff" is a logic term meaning "if and only if".]
 */
int inArray(int testNum, int array[], int n)
{
    int i = 0;
    int found = 0;
    while (!found && i < n) {
        found = array[i] == testNum;
        i += 1;
    }
    return found;
}

int main()
{
    int cmyColours[] = {CYAN, YELLOW, MAGENTA};
    const int NUM_CMYS = 3;

    for (Colour colour = RED; colour <= MAGENTA; colour += 1) {
        if (inArray(colour, cmyColours, NUM_CMYS)) {
            printf("%s is a CMY colour\n", colours[colour]);
        }
        else {
             printf("%s is an RGB colour\n", colours[colour]);
        }
    }
    return 0;
}
