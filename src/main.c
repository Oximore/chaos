#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
//#include "thread.h"
//#include "list.h"

void f(int a)
{
  printf("%d\n",a);
  return ;
}





int main()
{
  x
  thread_list = list_init();
  thread_t t;
  t = malloc(sizeof(struct thread));
  thread_create(&t,(void *(*)(void*))&f,(void *)14);
  list_add(thread_list,t);
  list_add(thread_list,t);
  print_list(thread_list);
  list_delete(thread_list);


  return EXIT_SUCCESS;
}






