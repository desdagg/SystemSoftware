// Orphan Example
// The child process is adopted by init process, when parent process dies.
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


#define START_HOUR  21;
#define START_MIN  32;
bool first_run = true;
bool isTestRun = true;

double getSleepTime();
 
int main()
{
    time_t now;
    double sleepTime;


    time(&now);  /* get current time; same as: now = time(NULL)  */
    
    // Create a child process      
    int pid = fork();
 
    if (pid > 0) {
        // if PID > 0 :: this is the parent
        // this process performs printf and finishes 
        printf("Parent process\n");
        sleep(10);  // uncomment to wait 10 seconds before process ends
        exit(EXIT_SUCCESS);
    } else if (pid == 0) {
       // Step 1: Create the orphan process
       printf("Child process\n");
       
       // Step 2: Elevate the orphan process to session leader, to loose controlling TTY
       // This command runs the process in a new session
       if (setsid() < 0) { exit(EXIT_FAILURE); }

       // We could fork here again , just to guarantee that the process is not a session leader
       /*
       int pid = fork();
       if (pid > 0) {
          exit(EXIT_SUCCESS);
       } else {
       */
          // Step 3: call umask() to set the file mode creation mask to 0
          // This will allow the daemon to read and write files 
          // with the permissions/access required 
          umask(0);

          // Step 4: Change the current working dir to root.
          // This will eliminate any issues of running on a mounted drive, 
          // that potentially could be removed etc..
          if (chdir("/") < 0 ) { exit(EXIT_FAILURE); }

          // Step 5: Close all open file descriptors
          /* Close all open file descriptors */
          int x;
          for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
          {
             //close (x);
          } 

          // Signal Handler goes here  #TODO

          // Log file goes here        #TODO

          // Orphan Logic goes here!! 
          // Keep process running with infinite loop
          // When the parent finishes after 10 seconds, 
          // the getppid() will return 1 as the parent (init process)
         
          while(1) {
	     sleep(1);
             printf("child 1: my parent is: %i\n", getppid());

	     // Report on the modified files since the last update
	     
	     // Set up the rules for the audit daemon (auditd)
	     if (first_run) {
	       setAuditRule();
	     }
	     //reportModifiedFiles();
	     
	     // Update the production site

	     
	      exit(EXIT_SUCCESS);
	     

	     //printf("before sleep time: %.f", sleepTime);
             sleepTime = getSleepTime(first_run);
	     first_run=false;

             printf("\nSleep for  %f", sleepTime);
	     sleep(sleepTime);
	     
	     // Audit modified files since last website update
	     // auditSandbox();
	     
	     // Update production site with new / modified files
	     updateWebsite();
	     
	     // Initiate backup of web site 
	     //backupSite();
	     
          }
      //}
    }
 
    return 0;
}


double getSleepTime(){
  printf("getSleepTime called\n");

  if (first_run) {
   

    time_t now;
    struct tm runTime;
    double seconds;
    time(&now);  
    runTime = *localtime(&now);
 
    if (isTestRun) 
      {
      runTime.tm_min = (now/60)%60+2; // Start 2 minutes later        
    }
    else {
     runTime.tm_min = START_MIN; 
    }
    runTime.tm_hour = START_HOUR; 
    runTime.tm_sec = 0;

    printf("Now Time %d : %d \n", (now/3600)%24,(now/60)%60 );
    printf("Run Time %d : %d ", runTime.tm_hour , runTime.tm_min);

    seconds =fabs(difftime(now,mktime(&runTime)));
    printf("\nSleeping  for  %.f", seconds);
    first_run = false;
    return seconds;
  }
  else{
    return 60;
  }

};

