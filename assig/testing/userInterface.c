#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main()
{
	int fd;
	char * fifoFile = "/home/des/Documents/systemSoftware/assignment/assig/testing/manualBackup";
	char input[10];
	char output[10];
	

	mkfifo(fifoFile, 0666);

	printf("Backup or transfer? [b/t] : ");
	scanf("%s", input);

	if(strcmp(input,"t") && strcmp(input,"b"))
	{
		printf("cancelled\n");
		return 0;
	}else if(!strcmp(input,"t")) {
		strcpy(output,"Transfer");
	}else {
		strcpy(output,"Backup");
	}
	
	printf("Backup started\n");
	fd = open(fifoFile, O_WRONLY);
	write(fd, output,sizeof(output));
	close(fd);
	unlink(fifoFile);

	return 0;
}
