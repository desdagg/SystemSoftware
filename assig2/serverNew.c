#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>
//#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>

#include "DBWrapper.h"

#define PORT 20000 
#define BACKLOG 5
#define MAX_BUF 512 
#define SITE_ROOT "/var/www/html/newSite/"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int readFile(int fd, char** userName, char** tmpFileName);

//int readDirCode(int nsockfd);
int parseUserData(int client_sockfd, char** userName, char** dirCode, char** fileName);
char** str_split(char* a_str, const char a_delim);
void *connection_handler(void *client_sock);
int copyFileToSite(char **tmpFileName,char **targetFileName, char **dirCode, char **destPath);

int main ()
{
	/* Defining Variables */
	int sockfd; 
	int client_sockfd; 
	int num , dirCode;
	int sin_size; 
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
  pthread_t thread_id;

	// Initialise the SQLite Database //
	init();

	
	/* Get the Socket file descriptor */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
		exit(1);
	}
	else 
		printf("[Server] Obtaining socket descriptor successfull.\n");

	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET;
	addr_local.sin_port = htons(PORT);
	addr_local.sin_addr.s_addr = INADDR_ANY; 

	/* Bind to Port */
	if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
		exit(1);
	}
	else 
		printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

	/* Listen for remote connect call from client */
	if(listen(sockfd,BACKLOG) == -1)
	{
		fprintf(stderr, "ERROR: Failed to listen. (errno = %d)\n", errno);
		exit(1);
	}
	else
		printf ("[Server] Listening the port %d successfully.\n", PORT);

	int success = 0;
	while(success == 0)
	{
		sin_size = sizeof(struct sockaddr_in);

		/* Wait for a connection */
		if ((client_sockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		{
		    fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else 
			printf("[Server] Server has got connected from %s with sockfd %d.\n", inet_ntoa(addr_remote.sin_addr),client_sockfd);
      //create a thread for each connection
      if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sockfd) < 0){
            perror("could not create thread");
            return 1;
      }
		//connection_handler(client_sockfd);
	}
	
	// Shutdown the Database 
	shutdownDB();
}


void *connection_handler(void *client_sock){
  int client_sockfd = *(int*)client_sock;

  char *userName;
  char *dirCode;
  char *fileName;
  char msg[MAX_BUF];
  char *tmpFileName;
  char *destPath;
  
  /* Receive User Credentials from Client */
  int reply =  parseUserData(client_sockfd,&userName, &dirCode, &fileName);
  printf("[server]  User details %s  %s  %s received from client \n",userName,dirCode,fileName);
   /*Receive File from Client */
  if(readFile(client_sockfd, &userName, &tmpFileName)){
    strcpy(msg,"File Received ");
  }else{
    strcpy(msg, "File Not Received ");
  }

  //Inform client that file was received
 
  send(client_sockfd, msg, MAX_BUF, 0);

  printf("[server] %s sent to client!\n",msg);
  //success = 1;
  
  //Copy received file to Correct WebSite Folder  (Switch statmenet based on dirCode)
  copyFileToSite(&tmpFileName, &fileName, &dirCode, &destPath);
  //Log the details to a database table
  logFileChanges(&userName, &destPath);


 

 
  close(client_sockfd);
  printf("[Server] Connection with Client closed. Server will wait now...\n");
  while(waitpid(-1, NULL, WNOHANG) > 0);
}




int parseUserData(int client_sockfd, char** userName, char** dirCode, char** fileName){
   char recvbuf[MAX_BUF]; // Receiver buffer
   char *details;
   char password[100];
   char strUserName[100];
   char **tmpStr;
   int valread;
   char *token;
   int authOK;
   
   /* RECEIVE credentials from client */
   if((valread = recv(client_sockfd,recvbuf,MAX_BUF,0) == -1)){
       perror("recv() on parseUserData");
     }

   printf("[server]  User details %s  Bytes %d received from client \n",recvbuf,valread);

   tmpStr = str_split(recvbuf,':');

   *userName = malloc(100);
   *dirCode = malloc(5);
   *fileName = malloc(100);
   
   printf("[server]  User details %s  %s  %s %s received from client \n", *(tmpStr+0),*(tmpStr+1) ,*(tmpStr+2) ,*(tmpStr+3));
      //printf("[server]  User details %s  %s  %s %s received from client \n",userName,password,dirCode,fileName);
   strcpy(*userName,*(tmpStr+0));
   strcpy(strUserName,*(tmpStr+0));
   
   strcpy(password,*(tmpStr+1));
   strcpy(*dirCode,*(tmpStr+2));
   strcpy(*fileName,*(tmpStr+3));
   printf("User name %s",*userName);
   

   //Check User credentials against database 
   authOK = authenticate(strUserName,password);
   
   char msg[20];
   if (authOK){
     strcpy(msg, "OK");
   } else {
     strcpy(msg,"NOT_OK"); 
   }
   
   printf("[server] Sending %s reply to client\n",msg);
   write(client_sockfd, msg,MAX_BUF);
     
   
   return 1;
}


int readFile(int client_sockfd, char** userName, char **tmpFileName){
  char recvbuf[MAX_BUF]; // Receiver buffer
  //char tmpFileName[100] = "/tmp/receive";
  *tmpFileName = malloc(100);
  
  strcpy(*tmpFileName, "/tmp/receive_");
  printf("readFile userName == %s\n", *userName);
  strcat(*tmpFileName, *userName);
  strcat(*tmpFileName, ".txt");


  printf("File name is == %s\n", *tmpFileName);
  FILE *fr = fopen(*tmpFileName, "w");
  if(fr == NULL)
    printf("File %s Cannot be opened file on server.\n", *tmpFileName);
  else
    {
      bzero(recvbuf, MAX_BUF); 
      int fr_block_sz = 0;
      /*
      if((valread = recv(client_sockfd,recvbuf,MAX_BUF-1,0) == -1)){
	perror("recv() on authenticate");
      }
      */
      
      while((fr_block_sz = recv(client_sockfd, recvbuf, MAX_BUF, 0)) > 0)
      	{
	  printf("recvbuf = %s %d\n",recvbuf,fr_block_sz);
	  int write_sz = fwrite(recvbuf, sizeof(char), fr_block_sz, fr);
	  if(write_sz < fr_block_sz)
	    {
	      error("File write failed on server.\n");
	      return 0;
	    }
	  bzero(recvbuf, MAX_BUF);
	  if (fr_block_sz == 0 || fr_block_sz != 512) 
	    {
	      break;
	    }
	}
      if(fr_block_sz < 0)
	{
	  if (errno == EAGAIN)
	    {
	      printf("recv() timed out.\n");
	      return 0;
	    }
	  else
	    {
	      fprintf(stderr, "recv() failed due to errno = %d\n", errno);
	      exit(1);
	    }
	}
      printf("[server] File received from client!\n");
      fclose(fr);
      return 1;
    }
  return 0;
}

//copy file to correct site directory
int copyFileToSite(char **tmpFileName,char **targetFileName, char **dirCode, char **destPath){
  char targetFolder[250];
  strcpy(targetFolder, SITE_ROOT);
  char cmd[400];
  //char caseVal[1];
  //strcpy(caseVal, *dirCode);
  int caseVal;
  //convert dirCode to int
  caseVal = atoi(*dirCode);
  printf("CaseVal is int : %d\n", caseVal);


  //get the target directory name using the dirCode
  switch(caseVal){
    case 1 :
      break;
    case 2 : 
      strcat(targetFolder, "Sales/");
      break;
    case 3 :
      strcat(targetFolder, "Marketing/");
      break;
    case 4 :
      strcat(targetFolder, "Promotions/");
      break;
    case 5 : 
      strcat(targetFolder, "Offers/");
      break;
    default :
      printf("Went into default ? "); 
      return 0;
  }

  *destPath = malloc(250);
  
  strcat(targetFolder, *targetFileName);
  strcpy(*destPath, targetFolder);
  printf("Destination File path %s ", targetFolder);
  strcpy(cmd, "cp ");
  strcat(cmd, *tmpFileName);
  strcat(cmd, " ");
  strcat(cmd, targetFolder);
  printf("System command %s \n", cmd);

  if (system(cmd) == -1){
    printf("Error executing copy :: %s\n", cmd);
    return 0;
  }

  return 1;

}

//split initial user unput string 
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

   
