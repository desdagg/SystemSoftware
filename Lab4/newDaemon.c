#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>



int main (int argc, char *argv[]) {
    int pid;
    int seconds;
    time_t now;

    printf("\nProgram Running...");

    pid = fork();

    printf("%i", pid);

    //Step1: Create Orphan
    if (pid > 0){
        printf("\nParent\n");
        sleep(10);
        exit(EXIT_SUCCESS);
    } else{

        //Step2: Set sessionID
        if(setsid() < 0) { exit(EXIT_FAILURE);}

        //Step3: unmask - file privileges to read and write
        umask(0);
		printf("\ngot here umask\n");
        //Step4: Change file directory
        if (chdir("/") < 0) {
			printf("chdir failed");
            exit(EXIT_FAILURE);
        }
		printf("chdir got here");
        //Step5: close file descriptors
        int x;
        for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
            close(x);
        }

        while(1) {
            struct tm newyear;
            time_t now = time(NULL);
            int diff;

            newyear = *localtime(&now);
            newyear.tm_hour	=	15;
            newyear.tm_min	=	28;

            diff = difftime(mktime(&newyear), now);
            printf("Time Difference: %i", diff);
            sleep(diff);

			char mode[] = "0000";            
			char buf[100] = "/home/des/Documents/Year_4/SystemSoftware/Lab4/temp";

			int i;
	        i = strtol(mode, 0, 8);
			printf("i is %d",i);
	        if (chmod (buf,i) < 0)
	        {
		  	 // do something if needed
	        }

            printf("File is changed.");
        }
    }
}
