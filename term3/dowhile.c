#include <stdio.h>

int main(void)
{
	int value;
	int i;
	do{
		scanf("%d", &value);
		printf("%d\n", value);
		i++;
	} while (value != 42 && i != -1);
	return 0;
}	
			
