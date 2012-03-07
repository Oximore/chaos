// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "list.h"


// Variables Globales
struct list thread_list;
struct thread *thread_current;


// Structures
struct thread{
  ucontext_t* context;
  int priority;
};

void thread_delete(thread_t t)
{
  struct thread * t2 = (struct thread *) t;
  free(t2->context->uc_stack.ss_sp);
  free(t2->context);
  free(t2);
}

int main()
{
  
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



extern int thread_create(struct thread **newthread, void *(*func)(void *), void *funcarg){
  struct thread* new_thread = malloc(sizeof(struct thread));
  new_thread->priority = 0;

  new_thread->context = malloc(sizeof(ucontext_t));

  getcontext(new_thread->context);
  thread_context.uc_stack.ss_size = 64*1024;
  thread_context.uc_stack.ss_sp    = malloc(thread_context.uc_stack.ss_size);
  thread_context.uc_link                =  NULL; 
  
  
  func(funcarg);
  
}





// pour le moment sans priorité
extern int thread_yield(void){
  // Sauvegarder le contexte courant, charger le suivant et changer le current
  
  
}


extern int thread_join(thread_t thread, void **retval);
extern void thread_exit(void *retval) __attribute__ ((__noreturn__));
*/
