#include "thread_mix.h"
#include <stdio.h>
#include <assert.h>

static void * threadfunc(void * arg)
{
  char *name = arg;
  printf("je suis le thread %p, lancé avec l'argument %s\n",
	 mthread_self(), name);
  mthread_yield();
  printf("je suis encore le thread %p, lancé avec l'argument %s\n",
	 mthread_self(), name);
  mthread_exit(arg);
  return NULL;
}

//int main(int argc, char *argv[])
int main()
{
  mthread_t thread1, thread2;
  void *retval1, *retval2;
  int err;

  printf("le main lance 2 threads...\n");
  err = mthread_create(&thread1, threadfunc, "thread1");
  assert(!err);
  err = mthread_create(&thread2, threadfunc, "thread2");
  assert(!err);
  printf("le main a lancé les threads %p et %p\n",
	 thread1, thread2);

  printf("le main attend les threads\n");
  err = mthread_join(thread2, &retval2);
  assert(!err);
  err = mthread_join(thread1, &retval1);
  assert(!err);
  printf("les threads ont terminé en renvoyant '%s' and '%s'\n",
	 (char *) retval1, (char *) retval2);

  return 0;
}
