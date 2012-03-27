#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"

void f(int a)
{
  printf("%d\n",a);
  return ;
}





int main()
{
  thread_t t;
  printf("Hello! \n");
  thread_create(&t,(void *(*)(void*))&f,(void *)14);
  printf("1\n");
  thread_yield();
  printf("2\n");






  /*list_add(thread_list,t);
  list_add(thread_list,t);
  print_list(thread_list);
  list_delete(thread_list);*/


  return EXIT_SUCCESS;
}






