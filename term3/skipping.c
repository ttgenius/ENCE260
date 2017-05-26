#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* skipping(const char* s);

int main(void){
	char* s = skipping("");
printf("%s\n", s);
free(s);}
	


char* skipping(const char* s){
	char* buff = malloc(sizeof(char));
	int i = 0;
	int j = 0;
	while(s[i] != '\0'){
		buff = realloc(buff, j+2);
		buff[j] = s[i];
		i+=1;
		if (s[i] != '\0'){
			i+=1;
			j+=1;
		} else{
			j+=1;
		}		
	}
	buff[j] = '\0';
	return buff;	
}	
    
