// Functions related to backing up the live site code

#include "backupSite.h"
#include "globals.h"

void backupSite(){
  // Write BACKUP_STARTED to message queue

  // Create a timestamped tar file of the LIVESITE_FOLDER and copy the tar file to the UPDATE_FOLDER   
	char bkpFilename[100];
	char bkpCommand[100];
	int BUFFSIZE = 1024;
	char buff[BUFFSIZE];
	int status;

	strcpy(bkpFilename, BACKUP_FOLDER);
	strcat(bkpFilename, "siteBackup_");
	strcat(bkpFilename, getBkpTStamp());
	strcpy(bkpCommand, "tar");
	strcat(bkpCommand, getBackupCommand(bkpFilename));

	printf("\nthe backup command is: %s\n",bkpCommand);
  
 
	
	FILE *fp = popen(bkpCommand, "r");
	while  (fgets(buff, BUFFSIZE, fp) != NULL){
		printf("\n%s\n",buff);
	}

	status = pclose(fp);
    	

	// Write BACKUP_ENDED to message queue
}

char* getBkpTStamp(struct tm myTime){
  time_t now;
  struct tm *time_now;
  static char buffer[100];
  
  time( &now );
  
  time_now = localtime( &now );
  
  strftime(buffer , 80,"%Y-%m-%d_%H_%M_%S", time_now);
  printf("BKP Formatted date & time : |%s|\n", buffer );
  return buffer;

}


char* getBackupCommand(char *filename){

  printf ("getBackupCommand called\n");
  static char str[100];
  strcpy(str, " -cvf ");
  strcat(str, filename);
  strcat(str, ".tar.gz ");
  strcat(str, LIVESITE_FOLDER);
  return str;
}

  


