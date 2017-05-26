#include <stdio.h>
#include <string.h>

int convertFromRoman(const char* s);

int main(void){
    char previous_token[81] = {'\0'};
    char current_token[81];
    int total = 0;
    while (scanf("%80s", current_token) > 0){
        if (((strcmp(current_token, "dn") == 0) || (strcmp(current_token, "dn,") == 0) || (strcmp(current_token, "dn.")== 0)) && (previous_token != '\0')){ //1.how to check dn dn. dn,  2.is there a file only containing dn or abc dn?
            int single_sum = convertFromRoman(previous_token);
            if (single_sum != -1){
				total += single_sum;
			}	
        } else {
            strncpy(previous_token, current_token, 80);
        }
    }       
    printf("%d\n", total);               
}   

int convertFromRoman(const char* s){
    int sum = 0;
    for (int i = 0; i < strlen(s); i++){
        if (s[i] == 'M'){
            sum += 1000;
        } else if (s[i] == 'D'){
            sum += 500;
        } else if (s[i] == 'C'){
            sum += 100;
        } else if (s[i] == 'L'){
            sum += 50;
        } else if (s[i] == 'X'){
            sum += 10;
        } else if (s[i] == 'V'){
            sum += 5;
        } else if (s[i] == 'I'){
            sum += 1;
        } else {
            return -1;                       
        }
    }
    return sum;       
}
