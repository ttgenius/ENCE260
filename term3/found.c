#include <stdio.h>

char* mystrrchr(char* s, int c);

int main(void){
	char* s = "ENCE260";
	char* foundAt = mystrrchr(s, 'E');
	if (foundAt == NULL) {
		puts("Not found");
	} else {
		printf("%zd", foundAt-s);
    }
}
	
char* mystrrchr(char* s, int c)
{
	char* found_at = NULL;
	while (*s != '\0'){
		if (*s == c){
			found_at = s;
		}	
	    s++;
	}

	return found_at;
}	     		
