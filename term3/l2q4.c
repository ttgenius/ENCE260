
				
#include <stdio.h>

int main(void)
{
    int value;
    scanf("%d", &value);
    if (value != 0) {
        if (value % 2 == 0){
        
            printf("Even");
        } else {
        
            printf("Odd");
        }
    } else {
        printf("Zero");
    }
}
