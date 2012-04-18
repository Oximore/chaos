
#include "thread_mix.h"
/*
#ifdef MODE_PTHREAD 
#include <pthread.h> 
#endif

#ifndef MODE_PTHREAD
#include "thread.h"
#endif
*/

extern mthread_t mthread_self(void) {
#ifdef MODE_PTHREAD 
  return pthread_self();
#endif
  
#ifndef MODE_PTHREAD
  return thread_self();
#endif
}

extern int mthread_create(mthread_t* newthread, void *(*func)(void *), void *funcarg) {
#ifdef MODE_PTHREAD 
  return pthread_create(newthread,NULL,func,funcarg);
#endif
  
#ifndef MODE_PTHREAD
  return thread_create(newthread,func,funcarg);
#endif
}

extern int mthread_yield(void) {
#ifdef MODE_PTHREAD 
  //  return pthread_yield(); // #define machin __GNUJESAISPASQUOI
  return sched_yield();
#endif
  
#ifndef MODE_PTHREAD
  return thread_yield();
#endif
}

extern int mthread_join(mthread_t thread, void **retval) {
#ifdef MODE_PTHREAD 
  return pthread_join(thread,retval);
#endif
  
#ifndef MODE_PTHREAD
  return thread_join(thread,retval);
#endif
}

extern void mthread_exit(void *retval) {
#ifdef MODE_PTHREAD 
  pthread_exit(retval);
#endif
  
#ifndef MODE_PTHREAD
  thread_exit(retval);
#endif
}


