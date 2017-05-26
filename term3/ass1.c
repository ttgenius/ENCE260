/*Find the peak accelerations*/

#include <stdio.h>

int main(void)
{
    int first_acc;
    int current_acc;
    int next_acc;
    int peak;
    int number = 0;
    int increase = 0;
  
  
    scanf("%d", &first_acc);
    scanf("%d", &current_acc);
  
    if (first_acc < current_acc){
        increase = 1;
    }  
  
    printf("%s  %s\n","Number","Magnitude");
  
    while (current_acc != -1){
        scanf("%d", &next_acc);
        if (next_acc > current_acc){
            current_acc = next_acc;
            increase = 1;  
        } else if (next_acc != -1 && next_acc < current_acc && increase == 1){
            peak = current_acc;
            number += 1;
            printf("%4d%11d\n", number, peak);
            current_acc = next_acc;
            increase = 0;
 
        } else {
            current_acc = next_acc;
        }    
       
    }
}        
