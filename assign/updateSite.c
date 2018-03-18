#include "updateSite.h"

bool updateWebsite()
{
  // Write Update Started message to Message Queue
  
  // Read list of files from latest Modified_files_ file inMODIFIED_LOGS_FOLDER
  char buffer[1024];
  char latest_file[100];
  
  FILE *fp = popen("ls -t | head -1","r");
  while (fgets(buffer,1024,fp) != NULL){
    printf("%s" , buffer);
    strcpy(latest_file,buffer);
  }
  
  // Read from latest_file 
  
  FILE *file = fopen(latest_file, "r");
  char line[256];
  while (fgets(line, sizeof(line), file)){
      // Add the file name ot an array ONLY if it is not already listed
      printf("%s\n",line);
    }
    
    //  For each file (path) in the array carry out a copy of that file to its same location but on the live site 
    // i.e. remove the file name only from the path using basename(path) and copy to LIVESITE_FOLDER
    
    char fileN[80];
    //fileN = basename(path); 
    
    
    
    printf("\n updateWebsite called\n");
    
    
    // Write Update finished message to message queue
    


 }
  
