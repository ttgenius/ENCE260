#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

/* **** CODE IS MISSING HERE *****/
typedef struct point_p Point;
typedef struct poly_y Polygon;

struct point_p{
	int x;
	int y;
	Point* next;
};	
	
struct poly_y{
	Point* vertices;
};	

Point* newPoint(int x, int y){
	Point* point = malloc(sizeof(Point));
	point->x=x;
	point->y=y;
	point->next =NULL;
	return point;
}	
			

Polygon* newPolygon(){
	Polygon* newPoly = malloc(sizeof(Polygon));
	newPoly->vertices = NULL;
	return newPoly;
}
	
void addVertex(Polygon* poly, Point* vertex){
	Point* p = poly->vertices;
	if(!p){
			poly->vertices = vertex;
	return;
}
		while (p->next){
			p=p->next;
		}
		p->next = vertex;
	    return;
}

void freePolygon(Polygon* poly){
		Point* vertex = poly->vertices;
		Point* next = NULL;
		while(vertex!=NULL){
			next = vertex->next;
			free(vertex);
			vertex = next;
		}
		free(poly);
}							
/* Print the given polygon on stdout, with one vertex per line */
void printPolygon(const Polygon* poly) {
    Point* p = poly->vertices;
    while (p) {
        printf("(%d, %d)\n", p->x, p->y);
        p = p->next;
    }
}

/* Read, print and free a polygon with checks for memory */
int main() {
    int allocatedBlocks = 0;
    int leakedBlocks = 0;
    allocatedBlocks = mallinfo().uordblks + mallinfo().hblkhd;

    Polygon* poly = newPolygon();
    int numVertices = 0;
    int i = 0;
    scanf("%d\n", &numVertices);
    for (i = 0; i < numVertices; i++) {
        int x = 0;
        int y = 0;
        scanf("%d %d\n", &x, &y);
        Point *vertex = newPoint(x, y);
        addVertex(poly, vertex);
    }

    printPolygon(poly);

    // The rest of the main function is done only in the second test
    freePolygon(poly);
    leakedBlocks = mallinfo().uordblks + mallinfo().hblkhd - allocatedBlocks;
    if (leakedBlocks != 0) {
        printf("Memory leak of %d bytes detected!\n", leakedBlocks);
    }
}
