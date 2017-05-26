#include <stdio.h>
int tokenCopy(char* dest, const char* src, int destSize);
int main(void){
	char buff[5];
	int n = tokenCopy(buff, "Thisis a string", 5);
	printf("%d '%s'\n", n, buff);
}
int tokenCopy(char* dest, const char* src, int destSize){
	int i = 0;
	while (src[i] != '\0' && i < destSize-1 && src[i] != ' '){
		dest[i] = src[i];
		i++;
		
	}
	dest[i] = '\0';
	return i;
}		
