#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
	//printf("Number: %d\n",atoi(argv[1]));
	
	int numbers[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
	char* numerals[] = {"I","IV","V","IX","X","XL","L","LC","C","CD","D","DM","M"};

	int num = atoi(argv[1]);
	printf("Numbeer: %d\n",num);

	char outString[64] = "";
	
	do{
		int i = 12;
		for (i ; i > -1 ; i--){
			printf("in for loop, num: %d\n", num);
			if (num >= numbers[i]){
				printf("in if %d, numbers[]: %d\n", i,numbers[i]);
				strncat(outString, numerals[i],32);
				printf("outString: %s, %s\n",outString, numerals[i]);
				num = num - numbers[i];
				printf("num is now: %d\n", num);
				i++;
			}
		}
		
	}while (num > 0);
	
	printf("numeral: %s\n",outString);
	
	return 0;
}
