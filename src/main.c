#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "structure.h"
#include "thread_tools.h"

void* f(int a)
{
  printf("%d\n",a);
  thread_yield();
  printf("%d\n",a*10);

  thread_exit(NULL);
  //  thread_yield();
  printf("%d\n",a*100);
  return NULL;
}


void print_list(struct data * d)
{
  if(d==NULL||d->list==NULL)
    return;
  struct node * n = d->list->first;
  while(n!=NULL)
    {
      printf("%d\n",n->thread->isfinished);
      n=n->left;
    }
}

void print_tree_rec(struct node * n)
{
  if(n==NULL)
    return;
  printf("%d\n",n->thread->isfinished);
  if(n->left !=NULL)
    print_tree_rec(n->left);
  if(n->right !=NULL)
    print_tree_rec(n->right);
}

void print_tree(struct data * d)
{
  if(d==NULL||d->tree==NULL)
    return;
  struct node * n = d->tree->root;
  print_tree_rec(n);
}



int main()
{
  /*thread_t t;
  thread_t t2;
  
   printf("Hello!%s \n","!!!");
  thread_create(&t,(void *(*)(void*))&f,(void *)10);
  printf("1\n");
  thread_create(&t2,(void *(*)(void*))&f,(void *)20);
  printf("2\n");
  thread_yield();
  printf("3\n");
  thread_join(t2,NULL);
  //thread_join(t,NULL);
  printf("5\n");*/
 
  thread_t t=malloc(sizeof(struct thread));
  thread_t t2=malloc(sizeof(struct thread));
  thread_t t3=malloc(sizeof(struct thread));
  thread_init(t);
  thread_init(t2);
  thread_init(t3);
  t3->isfinished = 1;
  struct data *d = data_init();
  if(data_add(d,t))
    {
      printf("erreur\n");
      return EXIT_FAILURE;
    }
  data_add(d,t2);
  data_add(d,t3);
  // get_lower_priority_thread(d);
  get_lower_priority_thread(d);
  thread_finished_delete(d,t3);
  print_list(d);
  print_tree(d);
  data_delete(d);

  /*list_add(thread_list,t);
  list_add(thread_list,t);
  print_list(thread_list);
  list_delete(thread_list);*/


  return EXIT_SUCCESS;
}






