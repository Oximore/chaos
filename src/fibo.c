#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

int affiche_adresse = 0;

void* fibo(void* arg);

int main(int argc, char* argv[]) {
  int n = 3;
  int res;
  if (argc>1)
    n = atoi(argv[1]);
  if (argc>2)
    affiche_adresse = 1;
  printf("n = %d\n",n);
  
  printf("une idée du tas : %p\n",malloc(sizeof(int)));

  thread_t th;
  thread_create(&th,&fibo,(void*)(n));
  thread_join(th,(void**)&res);  
  
  printf("\n\tfibo(%d) = %d\n",n,res);
  return EXIT_SUCCESS;
}

void* fibo(void* arg) {
  int n = (int) arg;
  int res1, res2;
  thread_t th1,th2;
  fprintf(stderr," %d",n);
  if (affiche_adresse)   fprintf(stderr,"(%p)",&n);
  if (n==0 || n==1) return (void*)1;
  else {
    thread_create(&th1,&fibo,(void*)(n-1));
    thread_create(&th2,&fibo,(void*)(n-2));
    thread_join(th1,(void**)&res1);
    thread_join(th2,(void**)&res2);
  }
  return (void*)(res1+res2);
}
