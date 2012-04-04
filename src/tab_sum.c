#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

void* tab_sum(void *arg);

int* tab;

struct argt{
  int i1;
  int i2;

};

int main(int argc, char* argv[])
{
  int n = 10;
  if(argc>1) n = atoi(argv[1]);
  if(n<=0) return 0;
  //on crée le tableau
  tab = malloc (n*sizeof(int));
  int i;  
  for(i=0;i<n;i++) tab[i]=i;
  
  int res;
  printf("n = %d\n", n);
 
 thread_t th;
 struct argt* argt = malloc(sizeof(struct argt));
 argt->i1=0;
 argt->i2=n-1;
 thread_create(&th,&tab_sum,(void*)(argt));
 thread_join(th,(void**)&res);
 
 printf("sum = %d\n",res);
 free(tab);
 free(argt);
 return EXIT_SUCCESS;	      
}



void* tab_sum(void* arg)
{
  int res1, res2=0;
  thread_t th1, th2;
  struct argt* argt = (struct argt*) arg;
  printf("indices: %d %d\n",argt->i1,argt->i1);
  if(argt->i1 == argt->i2) return (void*)tab[argt->i1];
  else
    {
      struct argt* arg1=malloc(sizeof(struct argt));
      struct argt* arg2=malloc(sizeof(struct argt));
      arg1->i1 = argt->i1;
      arg2->i2 = argt->i2;
      int m = (argt->i2 - argt->i1)/2;
      arg1->i2 = m;
      arg2->i1 = m+1;
      thread_create(&th1,&tab_sum,(void*)(arg1));
      thread_create(&th2,&tab_sum,(void*)(arg2));
      thread_join(th1,(void**)&res1); 
      thread_join(th1,(void**)&res1);
      free(arg1);
      free(arg2);
    }
  return (void*)(res1+res2);
}
