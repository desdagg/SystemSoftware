
#ifndef DBWRAPPER_H_
#define DBWRAPPER_H_


void init();

void shutdownDB();

int authenticate(char *name, char *password);

void logFileChanges(char **userName, char **targetFilePath);

#endif
