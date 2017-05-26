
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Declare the StringPair type.
// Note that we have incorporated the struct declaration into
// the typedef, but that this only works because we don't have any
// StringPair pointers in the structure (e.g. StringPair* next).
typedef struct stringpair_s {
    char* first;
    char* second;
 } StringPair;

// **** Insert your newStringPair function definition here ***

StringPair* newStringPair(const char* s1, const char* s2){
	size_t s1_length = strlen(s1);
	size_t s2_length = strlen(s2);
	StringPair* new_pair = malloc(sizeof(new_pair));
	
	
	
	


int main(void)
{
    char s1[] = "My first string";
    char s2[] = "Another one";
    StringPair* sp = NULL;

    sp = newStringPair(s1, s2);

    // Before printing, alter the initial strings to ensure
    // the function hasn't just copied the pointers.
    strncpy(s1, "Smasher1", sizeof(s1));
    strncpy(s2, "Clobber2", sizeof(s2));

    // Now print the new StringPair.
    printf("String pair: ('%s', '%s')\n", sp->first, sp->second);

    // Lastly free all dynamic memory involved.
    free(sp->first);
    free(sp->second);
    free(sp);
}
