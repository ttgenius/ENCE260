#include <stdio.h>

typedef struct vector2d_s Vector2d;

struct vector2d_s{
	int x;
	int y;
};

Vector2d vector(int x, int y){
	Vector2d vector = {x,y};
	return vector;
}

Vector2d vectorSum(Vector2d v1, Vector2d v2){
	
	Vector2d sum = {v1.x + v2.x,v1.y + v2.y};
	return sum;
}

int main(void){
	Vector2d v1 = vector(1, -97);
	Vector2d v2 = vector(1, 1);
	Vector2d v3 = vectorSum(v1, v2);
	printf("(%d, %d) + (%d, %d) = (%d, %d)\n",
			v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
	
}			
