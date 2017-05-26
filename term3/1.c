/* Simple array demo program.
 * Also demonstrates sizeof.
 * Reads a line of input characters into a 1D array
 * of 'char', then writes the line out backwards.
 * Written for ENCE260 by RJL.
 * February 2011, June 2013, July 2015, June 2016
 */

#include <stdio.h>
#include <stdlib.h>

#define N_MAX 100

int main(void)
{
	int data[100];
	char* b[100];
	printf("size of 100 int = %zd\n", sizeof(data));
	printf("size of 100 char pointers = %zu\n", sizeof(b));



    return EXIT_SUCCESS;
}
