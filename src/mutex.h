#ifndef __MUTEX_H__
#define __MUTEX_H__

struct mutex
{
  int lock;
};


struct mutex* mutex_create();
int mutex_lock(struct mutex *);
int mutex_trylock(struct mutex *);
int mutex_unlock(struct mutex *);
int mutex_destroy(struct mutex *);


#endif /* __MUTEX_H__ */
