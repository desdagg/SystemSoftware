#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>

void  main(void)
{
     pid_t  pid;

     pid = fork();
     if (pid == 0) // child
     {
       char* arg_list[] = {"ls","-l", NULL};
       printf ("Executing command ls...\n");
       execvp ("/bin/ls", arg_list);
     }
     else // parent
     {
       int status=0;
       wait(&status);
       printf ("Child process is returned with: %d.\n",status);
     }
}
