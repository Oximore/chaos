#include <stdio.h>
#include <stdlib.h>
#include "mutex.h"
#include <assert.h>

int main()
{
  return 0;
}

struct mutex* mutex_create()
{
  struct mutex * mutex=malloc(sizeof(struct mutex));
  assert(mutex!=NULL);
  mutex->lock=-1;
  return mutex;
}


int mutex_lock(struct mutex *mutex)
{
  mutex->lock=1;
  return 0;
}

int mutex_trylock(struct mutex *mutex)
{
  return mutex->lock;
}

int mutex_unlock(struct mutex *mutex)
{
  mutex->lock=0;
  return 0;
}

int mutex_destroy(struct mutex *mutex)
{
  if(mutex!=NULL) free(mutex);
  return 0;
}
