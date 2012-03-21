// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "list.h"


// Variables Globales
struct list *thread_list;
struct thread *thread_current;

void thread_delete(thread_t t);

void thread_delete(thread_t t)
{
  struct thread * t2 = (struct thread *) t;
  free(t2->context->uc_stack.ss_sp);
  free(t2->context);
  free(t2);
}

// Fonctions
/*extern thread_t thread_self(void){
  return thread_current;
}

//       int getcontext(ucontext_t *ucp);
//       int setcontext(const ucontext_t *ucp);
//       void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
//       int swapcontext(ucontext_t *oucp, ucontext_t *ucp);

*/
/*
int f(int adresse1,int adresse2){
  void* addr = sizeof(int)*adresse1 + adresse2;
  delet()
  }*/


void function(void *(func)(void *), void* funcarg){
  func(funcarg);
  
  thread_exit(NULL);// *TODO*
}



extern int thread_create(thread_t* new_thread, void *(*func)(void *), void *funcarg)
{
  thread_t nw_thread = *new_thread;
  nw_thread->priority = 0;
  nw_thread->context = malloc(sizeof(ucontext_t));
  getcontext(nw_thread->context);
  
  nw_thread->context->uc_stack.ss_size = 64*1024;
  nw_thread->context->uc_stack.ss_sp   = malloc(nw_thread->context->uc_stack.ss_size);
  nw_thread->context->uc_link                =  NULL; 
  
  makecontext(nw_thread->context, (void (*)(void)) function, 2, (int)func, (int)funcarg);
  return 0;  // *TODO* valeur de retour
}



// pour le moment sans priorité

  extern int thread_yield(void)
  {
    // Sauvegarder le contexte courant, charger le suivant et changer le current
    //struct list *thread_list;
    //struct thread *thread_current;
       
    thread_t tmp = get_lower_priority_thread(thread_list);
    
    if (tmp != NULL){
      getcontext(thread_current->context);
      list_add_last(thread_list, thread_current);
      thread_current = tmp; 
      setcontext(thread_current->context);
    }

    return 0;
  }


extern int thread_join(thread_t thread, void **retval){
  

  return 0;
}

//extern void thread_exit(void *retval) __attribute__ ((__noreturn__)){ 
extern void thread_exit(void *retval){ 
  exit(0); 
} 
