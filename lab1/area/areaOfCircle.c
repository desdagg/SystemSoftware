#include <stdio.h>
#include "areaOfCircle.h"

int areaOf_Circle(int r){
	int radius = r;
	float area;

	printf("radius: %d\n", radius);
	
	area = (float)(3.1416 * radius * radius);

	printf("area of Circle: %f\n", area);
}
