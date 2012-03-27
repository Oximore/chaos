#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"

void* f(int a)
{
  printf("%d\n",a);
  thread_yield();
  printf("%d\n",a*10);
  thread_exit(NULL);
  printf("%d\n",a*100);
  return NULL;
}





int main()
{
  thread_t t;
  thread_t t2;
  
  printf("Hello!%s \n","!!!");
  thread_create(&t,(void *(*)(void*))&f,(void *)10);
  printf("1\n");
  thread_create(&t2,(void *(*)(void*))&f,(void *)20);
  printf("2\n");
  thread_yield();
  printf("3\n");
  thread_join(t2,NULL);
  printf("4\n");
 






  /*list_add(thread_list,t);
  list_add(thread_list,t);
  print_list(thread_list);
  list_delete(thread_list);*/


  return EXIT_SUCCESS;
}






