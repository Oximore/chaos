#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

int affiche_adresse = 0;
int affiche_arg = 1;

void* fibo(void* arg);

int main(int argc, char* argv[]) {
  int n = 3;
  int res = 0;
  if (argc>1)
    n = atoi(argv[1]);
  if (argc>3)
    affiche_adresse = 1;
  else if (argc>2)
    affiche_arg = 0;
  printf("n = %d\n",n);
  
  printf("une idée du tas : %p\n",malloc(sizeof(int)));

  thread_t th;
  thread_create(&th,&fibo,(void*)(n));
  if (thread_join(th,(void**)&res))
    printf("Truuuu\n");
  
  printf("\n\tfibo(%d) = %d\n",n,res);
  return EXIT_SUCCESS;
}

void* fibo(void* arg) {
  int n = (int) arg;
  int res1 = 0, res2 = 0;
  thread_t th1,th2;
  if (affiche_arg)       fprintf(stderr," %d",n);
  if (affiche_adresse)   fprintf(stderr,"(%p)",&n);
  if (n==0 || n==1) return (void*)1;
  else {
    thread_create(&th1,&fibo,(void*)(n-1));
    thread_create(&th2,&fibo,(void*)(n-2));
    if (thread_join(th1,(void**)&res1))
      printf("thread_join 1 failed : %d\n", n);
    if (thread_join(th2,(void**)&res2))
      printf("thread_join 2 failed : %d\n", n);
  }
  return (void*)(res1+res2);
}
