// Utility functions related to time

#include "timeUtil.h"


double getSleepTime(bool first_run){
  printf("getSleepTime called\n");

  if (first_run) {

    time_t now;
    struct tm runTime;
    double seconds;
    bool isTestRun = TEST_RUN;

    time(&now);  /* get current time; same as: now = time(NULL)  */
    runTime = *localtime(&now);
    /*
    if (isTestRun) 
      {
      runTime.tm_min = (now/60)%60+2; // Start 2 minutes later        
    }
    else {
    */
    runTime.tm_hour = START_HOUR; 
    runTime.tm_min = START_MIN; 
    runTime.tm_sec = 0;

    printf("Now Time %d : %d \n", (now/3600)%24,(now/60)%60 );
    printf("Run Time %d : %d ", runTime.tm_hour , runTime.tm_min);

    seconds =fabs(difftime(now,mktime(&runTime)));
    printf("\nSleeping  for  %.f", seconds);
    first_run = false;
    return seconds;
  }
  else{
    //return 60*60*12;  // Run 24 Hours later
    return 60;  // Run 1 min later
  }
}


