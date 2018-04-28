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
#include <unistd.h>
#include <error.h>
#include <stdint.h>
#include <math.h>

#define PORT 20000
#define MAX_BUF 512 

int connectToServer();

//int authUser(int fd);

void sendFile( int fd);


int main(int argc, char *argv[])
{
  char buffer[MAX_BUF]; 
  int sockfd ;
  	int nsockfd;

	struct sockaddr_in remote_addr;

	/* Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
		exit(1);
	}
	printf("[Client] Connect To Server. \n");
	/* Fill the socket address struct */
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(PORT); 
	bzero(&(remote_addr.sin_zero), 8);

	/* Try to connect the remote server */
	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
		exit(1);
	}
	else 
		printf("[Client] Connected to server at port %d...ok!\n", PORT);
	

	// authUser(sockfd);
   
  sendFile(sockfd);
  int valread;
  valread = read (sockfd,buffer, MAX_BUF);
  printf("[Client] %s :: Received from SERVER\n",buffer);

  
	
  close (sockfd);
  printf("[Client] Connection lost.\n");
  return (0);
}

int connectToServer(){
  printf("[Client] Connect To Server.\n");
	/* Variable Definition */
	int sockfd; 
	int nsockfd;

	struct sockaddr_in remote_addr;

	/* Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
		exit(1);
	}
 printf("[Client] Connect To Server. 2\n");
	/* Fill the socket address struct */
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(PORT); 
	bzero(&(remote_addr.sin_zero), 8);

	/* Try to connect the remote server */
	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
		exit(1);
	}
	else 
		printf("[Client] Connected to server at port %d...ok!\n", PORT);

	return sockfd;
}


void sendFile (int sockfd){
  printf("authUser() called\n");
  char creds[MAX_BUF];
  char buffer[MAX_BUF];
  int valread;
  char usrStr[100];
  char pswdStr[100];
  char full_name[256];
  char sdbuf[MAX_BUF];

  char pathStr[100] ="";
  char fileStr[100] ="";
  char dirCode[5];

  printf("Enter User Name\n");
  scanf("\n%s",usrStr);
  printf("Enter Password Name\n");
  scanf("\n%s",pswdStr);

  

  printf("Enter Path to File (Enter . for cwd \n");
  scanf("\n%s",pathStr);
  printf("Enter FileName\n");
  scanf("\n%s",fileStr);

  if (strcmp(pathStr,".") ==0){
    strcpy(full_name,fileStr);

  } else {
    strcpy(full_name,pathStr);
    strcat(full_name,"/");
    strcat(full_name,fileStr);
  }

 
  //strcpy(full_name, getFullPath(pathStr,fileStr));
  printf("[Client] About to send %s to the Server... \n", full_name);
  printf("Select Destination Folder\n");
  printf("1 for Root Folder\n");
  printf("2 for Sales\n");
  printf("3 for Marketing\n");
  printf("4 for Promotions\n");
  printf("5 for Offers\n");
  printf("0 to Quit \n");
  scanf("\n%s",dirCode);
  
  strcpy(creds,usrStr);
  strcat(creds,":");
  strcat(creds,pswdStr);
  strcat(creds,":");
  strcat(creds,dirCode);
    strcat(creds,":");
  strcat(creds,fileStr);
  
  printf("[client] sending transfer details to server\n");

  /* SEND credentiats to server  */
  send(sockfd, creds, MAX_BUF,0);

  bzero(buffer,MAX_BUF);
  /* RECEIVE reply from server */
  valread = read(sockfd,buffer , MAX_BUF);
  printf("[client] %s  ::  Received from server\n" , buffer);  
  // shutdown(fd);

  if(strcmp(buffer,"NOT_OK") == 0){
  	printf("User credentiats invalid, closing connection\n");
  	close(sockfd);
  	exit(0);
  }
  
  // printf("\n Sending %s to server", dirCode);
  
  // send(sockfd, dirCode,MAX_BUF,0);
  // valread = read(fd,buffer , MAX_BUF);
  //printf("%s  ::  Received from server\n" , buffer);  
  
  
  printf("[Client] Sending %s to the Server...\n ", full_name);
  FILE *fs = fopen(full_name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", full_name);
      exit(1);
    }

  bzero(sdbuf, MAX_BUF); 
  int fs_block_sz; 
  while((fs_block_sz = fread(sdbuf, sizeof(char), MAX_BUF, fs)) > 0)
    {
      printf("sdbuf = %s %d\n",sdbuf,fs_block_sz);
      if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
	{
	  fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", full_name, errno);
	  break;
	}
      bzero(sdbuf, MAX_BUF);
    }
  printf("[client] Ok File %s from Client was Sent!\n", full_name);


}

/*
void sendFile(int sockfd)
{
  //  char* full_name = "/home/des/dev/assign2/test.txt";
  char full_name[256];
  char sdbuf[MAX_BUF];

  char pathStr[100] ="";
  char fileStr[100] ="";
  char dirCode[5];

  printf("Enter Path to File (Enter . for cwd \n");
  scanf("\n%s",pathStr);
  printf("Enter FileName\n");
  scanf("\n%s",fileStr);

  if (strcmp(pathStr,".") ==0){
    strcpy(full_name,fileStr);

  } else {
    strcpy(full_name,pathStr);
    strcat(full_name,"/");
    strcat(full_name,fileStr);
  }

 
  //strcpy(full_name, getFullPath(pathStr,fileStr));
  printf("[Client] About to send %s to the Server... \n", full_name);
  printf("Select Destination Folder\n");
  printf("1 for Root Folder\n");
  printf("2 for Sales\n");
  printf("3 for Marketing\n");
  printf("4 for Promotions\n");
  printf("5 for Offers\n");
  printf("0 to Quit \n");
  scanf("\n%s",dirCode);
  /*
  int SIZE = (int)((ceil(log10(dirCode))+1)*sizeof(char));
  char strNum[SIZE];
  sprintf(strNum , "%d",dirCode);
  

  
  
  printf("\n Sending %s to server", dirCode);
  
  send(sockfd, dirCode,MAX_BUF,0);
  // valread = read(fd,buffer , MAX_BUF);
  //printf("%s  ::  Received from server\n" , buffer);  
  
  
  printf("[Client] Sending %s to the Server... ", full_name);
  FILE *fs = fopen(full_name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", full_name);
      exit(1);
    }

  bzero(sdbuf, MAX_BUF); 
  int fs_block_sz; 
  while((fs_block_sz = fread(sdbuf, sizeof(char), MAX_BUF, fs)) > 0)
    {
      if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
	{
	  fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", full_name, errno);
	  break;
	}
      bzero(sdbuf, MAX_BUF);
    }
  printf("Ok File %s from Client was Sent!\n", full_name);
*/


