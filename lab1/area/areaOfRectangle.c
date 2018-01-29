#include <stdio.h>
#include "areaOfRectangle.h"

int areaOf_Rectangle(int s1, int s2){
	int area;
	int side1 = s1;
	int side2 = s2;
	
	printf("side 1: %d\n", side1);
	printf("side 2: %d\n", side2);

	area = side1 * side2;

	printf("area of rectangle: %d\n", area);

	return area;
}
