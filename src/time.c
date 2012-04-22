#include <sys/time.h>
#include <stdio.h>
#include "time.h"

struct timeval tv1, tv2;

int time_start(void) {
#ifdef TIME_MODE
  if (gettimeofday(&tv1, NULL))
    return -1;
  printf("START\n");
#endif
  return 0;
}

int time_end(void) {
#ifdef TIME_MODE
  unsigned long microseconds;
  if (gettimeofday(&tv2, NULL))
    return -1;
  microseconds = (tv2.tv_sec-tv1.tv_sec)*1000000 +(tv2.tv_usec-tv1.tv_usec);
  printf("END\t");
  printf("temps d'exécution : %ld microsecondes\n", microseconds);
#endif
  return 0;
}
