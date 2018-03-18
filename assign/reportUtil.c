// Utility functions related to identifying and reporting modified files to the web site 

#include "globals.h"
#include "reportUtil.h"

char* CLEAR_AUDIT_RULES = "auditctl -D";


int isFilename(const char* path);

void setAuditRule(){
  printf("setAuditRule called\n");  

  FILE *fp;
  int status;
  char buffer[1024];
  char command[100];
  char params[]   = " -p w -k ";

  strcpy(command, "auditctl -w ");
  strcat(command,SANDBOX_FOLDER);
  strcat(command,params);
  strcat(command,AUDIT_KEY);

  printf("\nExecute Command : %s\n" , command);
  /*
  // Clear the rules from auditd
  if ((fp = popen(CLEAR_AUDIT_RULES, "r")) == NULL) {
    printf("Error opening pipe for %s\n", CLEAR_AUDIT_RULES);
    }
  */ 

  if ((fp = popen(command, "r")) == NULL) {
    printf("Error opening pipe for %s\n", command);
    // #TODO Add Error Logging here
  }
    status = pclose(fp);
}


void reportModifiedFiles() {
  printf ("reportModifiedFiles called\n");
  time_t now = time(NULL);
  int BUFFSIZE = 1024;
  char *command;
  char modFilename[100];
  char buff[BUFFSIZE];
  FILE *fp_mod;
  char *tmpStr;

  // Get a list of files modified since the last update at lastUpdate_time
  
  // Use ausearch and aureport to get modified files since midnight
  // #TODO Pass last update time to teh following function 
  command = getAuditReportCommand("today");
  printf ("\n Audit Report Cmd : %s\n" ,command);  


  strcpy(modFilename, MODIFIED_LOGS_FOLDER);
  strcat(modFilename, "Files_Modified_");
  strcat(modFilename, getTStamp());
  strcat(modFilename, ".txt");
  printf ("\n Mod Filename :  %s \n", modFilename );

  
  // Open a new file to write modified files to
  fp_mod=fopen(modFilename, "w+");  
  fprintf(fp_mod,"Modified Files and users\n");


  FILE *fp=popen(command, "r"); 
  while  (fgets(buff, BUFFSIZE, fp) != NULL){
    printf("BUFFER :  %s" ,buff);
    tmpStr = strdup(buff);
    // Get the first element in the tokenised string
    char *fileN =strsep(&tmpStr,":");
    printf("\nFile Name %s", fileN);
	struct stat path_stat;
	stat(fileN, &path_stat);
	if (S_ISREG(path_stat.st_mode))	{
		fprintf(fp_mod,buff);
		printf(" added to output file\n");	
	} else{
	    printf("/n%s does not exist\n", fileN);
	}
}
  fclose(fp_mod);
  
  pclose(fp);
  
  	
}


int isFilename(const char* path){
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}


/*
int isDirectory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) !=0)
}

char* getFileName(); //char* prefix){
    char filename[80];
    struct tm *timenow;
    //   strcat(prefix,filename);
    time_t now = time(NULL);
    timenow = gmtime(&now);
    
    strftime(filename, sizeof(filename), "/var/log/SA_TEST_%Y-%m-%d_%H:%M:%S", timenow);
    printf("\n Filename : %s" , filename);
    return filename;
    

    // fopen(filename,"w");
  }

}
*/

char* getTStamp(struct tm myTime){
  time_t now;
  struct tm *time_now;
  static char buffer[80];
  
  time( &now );
  
  time_now = localtime( &now );
  
  strftime(buffer , 80,"%Y-%m-%d_%H_%M_%S", time_now);
  printf("Formatted date & time : |%s|\n", buffer );
  return buffer;

}


char* getAuditReportCommand(char *timestamp){
  printf ("getAuditReportCommand called\n");
  static char str[80];
  strcpy(str, "ausearch -ts ");
  strcat(str, timestamp);
  strcat(str, " -k ");
  strcat(str, AUDIT_KEY);
  strcat(str, " | aureport -i -f | awk 'length($4)>2 {print $4\":\"$8}'");
  return str;
}

  
