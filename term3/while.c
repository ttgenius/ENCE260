
/* Print a geometric progression, with the start value, multiplier,
 * and number of elements read from standard input.
 * Written by Richard Lobb, June 2012, for ENCE 260.
 */
#include <stdio.h>

int main(void)
{
	int value;
	int i;
	while (value != 42 && i != -1){
		scanf("%d", &value);
		printf("%d\n", value);
		i++;
    }
    return 0;
}       		
		

