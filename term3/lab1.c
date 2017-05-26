#include <stdio.h>
#include <stdlib.h>

#define FACTOR 1.609344

int main(void)
{
	float miles = 0;
	float km = 0;
	printf("How many miles? ");
    scanf("%f", &miles);
	km = miles * FACTOR;
	
	
	printf("That's %.2f km.\n", km);
	return EXIT_SUCCESS;
}
