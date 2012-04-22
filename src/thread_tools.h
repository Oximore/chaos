#ifndef __THREAD_TOOLS__
#define __THREAD_TOOLS__

#include "thread.h"

//int thread_init(thread_t); // Vraiment besoin ?
int thread_isfinished(thread_t);
int thread_getpriority(thread_t);
int thread_delete(thread_t);


#endif /*__THREAD_TOOLS__*/
