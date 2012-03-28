// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread_tools.h"
#include "list.h"

#define VALEUR_MODE_DEBUG 0
#ifdef MODE_DEBUG
#define VALEUR_MODE_DEBUG 1
#endif
#define debug(arg) if (VALEUR_MODE_DEBUG) fprintf(stderr, "%s l.%d: %s\n", __FILE__, __LINE__, arg)


// Structures
struct thread{
  int priority;
  ucontext_t* context;
  void * retval;
  int isjoined;
  struct thread* joiner;
  int isfinished;
};


// Variables Globales
struct list* thread_list = NULL;
struct thread* thread_current = NULL;

//       int getcontext(ucontext_t *ucp);
//       int setcontext(const ucontext_t *ucp);
//       void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
//       int swapcontext(ucontext_t *oucp, ucontext_t *ucp);


int thread_delete(struct thread* thread_to_del);
void function(void *(func)(void *), void* funcarg);

// Fonctions
extern struct thread* thread_self(void) {
  debug("thread_self");
  return thread_current;
}


void function(void *(func)(void *), void* funcarg) {
  debug("function");
  void * retour = func(funcarg);
  thread_current->retval = retour ;
  thread_current->isfinished = 1;
 
  // Si on l'attend on passe la main et le thread_join le détruira
  if ( thread_current->isjoined )
    thread_yield();

  // sinon on le détruit 
  struct thread* tmp = get_lower_priority_thread(thread_list);
  
  if ( tmp == NULL)
    exit(0);
  thread_delete(thread_current);
  thread_current = tmp;
  setcontext(thread_current->context);
}


extern int thread_create(struct thread** new_thread, void *(*func)(void *), void *funcarg) {
  debug("thread_create");
  
  if (thread_current == NULL) {
    // Si c'est la première fois qu'on crée un thread
    // On alloue le thread principal
    struct thread* main_thread = malloc(sizeof(struct thread));
    main_thread->priority = 0;
    main_thread->isfinished = 0;
    main_thread->joiner = NULL;
    main_thread->context = malloc(sizeof(ucontext_t));
    getcontext(main_thread->context);

    // Malloc t'on la pile ?
    main_thread->context->uc_stack.ss_size = 64*1024;
    main_thread->context->uc_stack.ss_sp   = malloc(main_thread->context->uc_stack.ss_size);
    main_thread->context->uc_link =  NULL; 
    main_thread->retval = NULL;
    main_thread->isjoined = 0;
    thread_current = main_thread;
    
    // Puis on crée la thread_list
    thread_list = list_init(); 
  }
  
  *new_thread = malloc(sizeof(struct thread));
  (*new_thread)->priority = 0;
  (*new_thread)->isfinished = 0;
  (*new_thread)->joiner = NULL;
  (*new_thread)->context = malloc(sizeof(ucontext_t));
  getcontext((*new_thread)->context);

  (*new_thread)->context->uc_stack.ss_size = 64*1024;
  (*new_thread)->context->uc_stack.ss_sp   = malloc((*new_thread)->context->uc_stack.ss_size);
  (*new_thread)->context->uc_link =  NULL; 
  (*new_thread)->retval = NULL;
  (*new_thread)->isjoined = 0;
  list_add_last(thread_list,(*new_thread));
  
  makecontext((*new_thread)->context, (void (*)(void)) function, 2, (int)func, (int)funcarg);
  return 0;  // *TODO* valeur de retour
}



// pour le moment sans priorité
extern int thread_yield (void) {
  debug("thread_yield");
  // Sauvegarder le contexte courant, charger le suivant et changer le current
  struct thread* tmp = get_lower_priority_thread(thread_list);
  struct thread* current;

  if (tmp != NULL){
    current = thread_current;
    list_add_last(thread_list, thread_current);
    thread_current = tmp; 
    
    int i = 0;
    if (!current->isfinished)
      getcontext(current->context);
    
    if (i==0){
      i++;
      setcontext(thread_current->context);
    }    
    i = 0;
    return 0;
  }
  return -1;
}


extern int thread_join(struct thread* thread, void **retval){
  debug("thread_join");
  thread->isjoined = 1;
  thread->joiner = thread_current;
  
  // passer la main
  while (!thread->isfinished) // supp le while plus tard
    thread_yield();
  

  
  if (retval != NULL)
    *retval = thread->retval;
  thread->isjoined = 0;
  
  // On supprime ce thread
  list_element_delete(thread_list, thread);
  thread_delete(thread);
  
  return 0; // *TODO*
}

int thread_delete(struct thread* thread_to_del){
  debug("thread_delete");
  if (thread_to_del != NULL){
    if (thread_to_del->context != NULL){
      if (thread_to_del->context->uc_stack.ss_sp != NULL){
	free(thread_to_del->context->uc_stack.ss_sp);
      }
      free(thread_to_del->context);
    }     
    free(thread_to_del);
    return 0;
  }
  else
    return 1;
}

//extern void thread_exit(void *retval) __attribute__ ((__noreturn__)){ 
extern void thread_exit(void *retval){ 
  debug("thread_exit");
  // Si l'on a jamais appelé thread_create
  if (thread_list == NULL)
    exit(0);
  
  // Sinon, si on l'attend
  thread_current->isfinished = 1;
  thread_current->retval = retval;   
  if (thread_current->isjoined){ 
    thread_yield();
  }
  
  struct thread* tmp = get_lower_priority_thread(thread_list);
  // Si il n'y a plus de thread executable, alors c'est la fin
  if ( tmp  == NULL)
    exit(0);
  list_add(thread_list,tmp);

  if (thread_current->context != NULL){
    if (thread_current->context->uc_stack.ss_sp != NULL){
      free(thread_current->context->uc_stack.ss_sp);
    } 
    free(thread_current->context);
  } 
  thread_current->context = NULL;
    
  // si thread_current->joiner != NULL lui passer la main ??
  // Ou 
  thread_yield();
} 

int thread_isfinished(struct thread* thread){
  return thread->isfinished;
}

int thread_getpriority(struct thread* thread){
  return thread->priority;
}
