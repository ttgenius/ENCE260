/* A program to demonstrate the (mis)use of external
 * variables. Reads a name (well, any string of chars,
 * really), converts it to upper case, then prints it
 * out.
 * Written for ENCE260, June 2011/July 2015
 * Author: Richard Lobb
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 80
char name[MAX_NAME_LENGTH];

// Read a name (or any string) into the external "name" array.
// Terminate it with null.

int main(void)
{
	int c = EOF;
	printf("%o", c);
}
