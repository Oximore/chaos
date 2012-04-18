#ifndef THREAD_MIX_H
#define THREAD_MIX_H

#ifdef MODE_PTHREAD 
#include <pthread.h> 
#include <sched.h>
typedef pthread_t mthread_t;
#endif

#ifndef MODE_PTHREAD
#include "thread.h"
typedef thread_t mthread_t;
#endif

extern mthread_t mthread_self(void);
extern int mthread_create(mthread_t* newthread, void *(*func)(void *), void *funcarg);
extern int mthread_yield(void);
extern int mthread_join(mthread_t thread, void **retval);
extern void mthread_exit(void *retval)/* __attribute__ ((__noreturn__))*/;

#endif /* __THREAD_MIX_H__ */
