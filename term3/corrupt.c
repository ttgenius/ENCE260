/* Program to demonstrate various heap mismanagement problems.
 * Reads lines from a given file into an array of strings,
 * prints them all out and frees them.
 * BUGGY!
 * Written for COSC208, Richard Lobb, 2007, 2008, 2009; ENCE260 2012/3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 100

// Return a copy of the string s
char* copyString(char* s) {
    int len = strlen(s);
    char* copy = malloc(len+1); // len+1
    strncpy(copy, s, len);
    return copy;
}

/* Read lines from the file into a dynamically allocated array of strings.
 * Returns a pointer to the array of strings, which is terminated by a NULL.
 * Each string will usually end with a newline character.
 */
char** readFile(FILE *fp) {
    char** stringBuffer = malloc(sizeof(char*));
    int numStrings = 0;
    char line[MAX_LINE_SIZE];
    stringBuffer[0] = NULL;//derefece address that doesn't exist
    while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        stringBuffer = realloc(stringBuffer, (numStrings+2)* sizeof(char*));
        stringBuffer[numStrings] = copyString(line);
        numStrings += 1;
    }
    stringBuffer[numStrings] = NULL;  // Terminator
    return stringBuffer;
}

// Print all the strings in the given string array (terminated by NULL)
void printStrings(char* s[]) {
    int i = 0;
    while (s[i] != NULL) {
        printf("%s", s[i]);
        i += 1;
    }
}

// Free all the strings in the given string array (terminated by NULL)
void freeAllStrings(char* s[]) {
    int i = 0;
    while (s[i] != NULL) {
        free(s[i]);
        i += 1;
    }
}


int main(int argc, char* argv[])
{
    FILE* inputFile = NULL;
    char** stringArray = NULL;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }
    inputFile = fopen(argv[1], "r"); /* Open file for reading */
    if (inputFile == NULL) {
        fprintf(stderr, "Can't open file '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }
    stringArray = readFile(inputFile);
    printStrings(stringArray);
    freeAllStrings(stringArray);
    free(stringArray);
    fclose(inputFile);
    return EXIT_SUCCESS;
}
