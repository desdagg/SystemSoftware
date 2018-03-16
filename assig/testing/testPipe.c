#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_BUF 1024

int main()
{
	int fd;
	char * fifoFile = "/home/des/Documents/systemSoftware/assignment/assig/testing/manualBackup";
	char buf[MAX_BUF];

	fd = open(fifoFile, O_RDONLY);
	read(fd, buf, MAX_BUF);
	printf("Message in: %s\n",buf);
	close(fd);

	return 0;
}
