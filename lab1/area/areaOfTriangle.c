#include <stdio.h>
#include "areaOfTriangle.h"

int areaOf_Triangle(int h, int b){
	float area;
	float height = h;
	float base = b;

	printf("height: %.2f\n", height);
	printf("base: %.2f\n", base);

	area = (float)(base/2) * height;

	printf("area of triangle: %.2f\n", area);
}
