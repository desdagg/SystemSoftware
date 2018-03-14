#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd;
	char * fifoFile = "/home/des/Documents/Year_4/SystemSoftware/Lab5/tmp/fifoFile";
	
	mkfifo(fifoFile, 0666);
	
	fd = open(fifoFile, O_WRONLY);
	write(fd, "De trut is out dere!!", sizeof("De trut is out dere!!"));
	close(fd);

	unlink(fifoFile);

	return 0;
}
