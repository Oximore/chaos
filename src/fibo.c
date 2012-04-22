#include <stdio.h>
#include <stdlib.h>
#include "thread_mix.h"
#include "time.h"

int affiche_adresse = 0;
int affiche_arg = 0;

void* fibo(void* arg);

int main(int argc, char* argv[]) {
  int n = 3;
  int res = 0;
  
  // Arg Filter
  if (argc>1)
    n = atoi(argv[1]);
  if (argc>2)
    affiche_arg = 1;
  if (argc>3)
    affiche_adresse = 1;

  printf("n = %d\n",n);
  if (affiche_adresse)
    printf("une idée du tas : %p\n",malloc(sizeof(int)));
  
  time_start(); {
    mthread_t th;
    mthread_create(&th,&fibo,(void*)(n));
    if (mthread_join(th,(void**)&res))
      printf("Echec de join\n");
    
    if (affiche_arg)
      printf("\n");
    printf("\tfibo(%d) = %d\n",n,res);
  }
  time_end();
  return EXIT_SUCCESS;
}

void* fibo(void* arg) {
  int n = (int) arg;
  int res1 = 0, res2 = 0;
  mthread_t th1,th2;
  if (affiche_arg)       fprintf(stderr," %d",n);
  if (affiche_adresse)   fprintf(stderr,"(%p)",&n);
  if (n==0 || n==1) return (void*)1;
  else {
    mthread_create(&th1, &fibo, (void*)(n-1));
    mthread_create(&th2, &fibo, (void*)(n-2));
    if (mthread_join(th1,(void**)&res1))
      printf("thread_join 1 failed : %d\n", n);
    if (mthread_join(th2,(void**)&res2))
      printf("thread_join 2 failed : %d\n", n);
  }
  return (void*)(res1+res2);
}
