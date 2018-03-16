#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define NUM_ERROR = "number too big"

void main(int argc, char *argv[]){
	int num;
	num = atoi(argv[1]);

	printf("value: %d\n", num);

	char* numeral = getNumeral(num);

	printf("numeral: %s\n", numeral);
	return 0;
}


char* getNumeral(int num){
	if(num > 3999){
		return "NUM_ERROR";
	}
	
	char output[255] = "";
	int temp = num;

	do{
		strcat(output, convertNumber(temp));
		printf("%d\n", temp);
		printf("%s\n", output);
		temp = num - temp;
	}while (temp > 0);

	return output;
}


char* convertNumber(int num){
	int a = 13;
	char* returnNumeral = "";
	static int decimal[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
	static char* roman[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
	
	for (int i = a; --i >=0;){
		if(num > decimal[a]){
			returnNumeral = roman[i];
			break;
		}
	}
	return returnNumeral;
}
