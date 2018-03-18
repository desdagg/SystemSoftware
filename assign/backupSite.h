// Utility functions related to auditing and reporting the changes to the web site since the last update

#ifndef BACKUPSITE_H_
#define BACKUPSITE_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

void backupSite();

char* getBkpTStamp();

char* getBackupCommand();

#endif // BACKUPSITE_H_
