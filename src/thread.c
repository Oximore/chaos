// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "list.h"


// Variables Globales
struct list *thread_list;
struct thread *thread_current;


void thread_delete(thread_t t)
{
  struct thread * t2 = (struct thread *) t;
  free(t2->context->uc_stack.ss_sp);
  free(t2->context);
  free(t2);
}
void f(int a)
{
  printf("%d\n",a);
  return ;
}

int main()
{
  thread_list = list_init();
  thread_t t;
  t = malloc(sizeof(struct thread));
  thread_create(t,(void *(*)(void*))&f,(void *)14);
  list_add(thread_list,t);
  list_add(thread_list,t);
  print_list(thread_list);
  list_delete(thread_list);
  return EXIT_SUCCESS;
}
// Fonctions
/*extern thread_t thread_self(void){
  return thread_current;
}

//       int getcontext(ucontext_t *ucp);
//       int setcontext(const ucontext_t *ucp);
//       void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
//       int swapcontext(ucontext_t *oucp, ucontext_t *ucp);


int f(int adresse1,int adresse2){
  void* addr = sizeof(int)*adresse1 + adresse2;
  
  
  
  
}


*/



extern int thread_create(thread_t new_thread, void *(*func)(void *), void *funcarg)
{
  new_thread->priority = 0;
  new_thread->context = malloc(sizeof(ucontext_t));
  getcontext(new_thread->context);
  
  new_thread->context->uc_stack.ss_size = 64*1024;
  new_thread->context->uc_stack.ss_sp    = malloc(new_thread->context->uc_stack.ss_size);
  new_thread->context->uc_link                =  NULL; 
  makecontext(new_thread->context,(void (*)(void)) func,(int)funcarg);
  return 1;
}



// pour le moment sans priorité

  extern int thread_yield(void)
  {
    // Sauvegarder le contexte courant, charger le suivant et changer le current
    
    ucontext_t *ucp = malloc(sizeof(ucontext_t));
    getcontext(ucp);
    thread_t next = get_lower_priority_thread(thread_list);
    return swapcontext(next->context, ucp);
  }


extern int thread_join(thread_t thread, void **retval);
extern void thread_exit(void *retval) __attribute__ ((__noreturn__));

