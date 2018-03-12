#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

void set_file_permissions();

void set_file_permissions(){
    struct stat st;
    mode_t mode;
    const char *path = "/Users/paulbrittain/CLionProjects/systemssoftware/Lab4/file_permission_change.txt";

    stat(path, &st);

    mode = st.st_mode & 07777;

    // Change permission mode.
    mode &= ~(S_IRUSR);
    mode |= S_IXUSR;

    chmod(path, mode);
}

int main (int argc, char *argv[]) {
    int pid;
    int seconds;
    time_t now;

    printf("\nProgram Running...");

    pid = fork();

    printf("%i", pid);

    //Step1: Create Orphan
    if (pid > 0){
        printf("\nParent");
        sleep(10);
        exit(EXIT_SUCCESS);
    } else{

        //Step2: Set sessionID
        if(setsid() < 0) { exit(EXIT_FAILURE);}

        //Step3: unmask - file privileges to read and write
        umask(0);

        //Step4: Change file directory
        if (chdir("/") < 0) {
            exit(EXIT_FAILURE);
        }

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
            set_file_permissions();
            printf("File is changed.");
        }
    }
}
