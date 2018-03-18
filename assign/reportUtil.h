// Utility functions related to auditing and reporting the changes to the web site since the last update

#ifndef REPORTUTIL_H_
#define REPORTUTIL_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>


void setAuditRule();

void reportModifiedFiles();

char* getFileName();

char* getTStamp();

char* getAuditReportCommand(char* timestamp);

//int isFilename(const char* filename);

#endif //  REPORTUTIL_H_
