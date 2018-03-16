// Utility functions related to identifying and reporting modified files to the web site 

#include "reportUtil.h"


void setAuditRule(){
  FILE *fp;
  int status;
  char buffer[1024];

  char command[]   = "auditctl -w ";
  char watch_key[] = " -p w -k des_watch";

  printf("printAuditRule called\n");  
  
  strcat(command,SANDBOX_FOLDER);
  strcat(command,watch_key);
  printf("\nExecute Command : %s" , command);
  
  if ((fp = popen(command, "r")) == NULL) {
    printf("Error opening pipe for %s\n", command);
    // #TODO Add Errror Logging here
  }

  status = pclose(fp);
}


void reportModifiedFiles() {

  printf ("reportModifiedFiles called");


}


