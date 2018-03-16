// Utility functions related to auditing and reporting the changes to the web site since the last update

#ifndef REPORTUTIL_H_
#define REPORTUTIL_H_

#include <stdio.h>
#include <string.h>

const char* SANDBOX_FOLDER = "/var/tmp/sandbox";


void setAuditRule();

void reportModifiedFiles();

#endif //  REPORTUTIL_H_
