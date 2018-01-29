#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "areaOfRectangle.h"
#include "areaOfTriangle.h"
#include "areaOfCircle.h"

int main(int argc, char *argv[]){
	
	if(strcmp(argv[1], "Rectangle") == 0){
		printf("argv: %s\n", argv[1]);
		areaOf_Rectangle(atoi(argv[2]), atoi(argv[3]));
	}else if(strcmp(argv[1], "Triangle") == 0){
		areaOf_Triangle(atoi(argv[2]), atoi(argv[3]));
	}else if(strcmp(argv[1], "Circle") == 0){
		areaOf_Circle(atoi(argv[2]));
	}
}
