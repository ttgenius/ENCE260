#include <stdio.h>

int tokenCopy(char* dest, const char* src, int destSize);
int main(void){
	char buff[3];
	int n = tokenCopy(buff, "This is a string", 3);
	printf("%d '%s'\n", n, buff);
}

		
int tokenCopy(char* dest, const char* src, int destSize){
    char* copy= dest;
	while (*src != '\0' && src < &dest[destSize-1] && *src != ' '){
		
		*dest = *src;
		 src++;
		 dest++;
		
		 
	}
	*dest = '\0';
	return dest-copy ;
}		 
