// Utility functions related to time

#ifndef TIMEUTIL_H_
#define TIMEUTIL_H_

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define START_HOUR  21;
#define START_MIN  28;
#define TEST_RUN true;

double getSleepTime(bool first_run);

#endif //  TIMEUTIL_H_
