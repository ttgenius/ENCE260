#include <stdio.h>
#include <math.h>

int main(void)
{
	float a;
	float b;
	float c;
	float delta;
	scanf("%f %f %f", &a, &b, &c);
	delta = b * b - 4 * a * c;
	if (a == 0){
		printf("Not a quadratic");
	} else if (delta < 0){
		printf("Imaginary roots");
	} else {
		float root_1 = (- b - sqrt(delta)) / (2 * a);
		float root_2 = (- b + sqrt(delta)) / (2 * a);
		
		if (root_1 <= root_2){
			printf("Roots are %.4f and %.4f", root_1, root_2);
		} else{
			printf("Roots are %.4f and %.4f", root_2, root_1);	
        }  
	}
}		
		
	
	   
	
