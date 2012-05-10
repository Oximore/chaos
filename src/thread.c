// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#ifdef MODE_DEBUG 
//#include <valgrind/valgrind.h>
#endif

#include "thread_tools.h"
#include "structure.h"


// Structures
struct thread {
  int priority;
  ucontext_t* context;
  void * retval;
  struct thread* joiner;
  int isfinished;
  //#ifdef MODE_DEBUG 
  //int valgrind_stackid;
  //#endif
};


// Variables Globales
structure      thread_set     = NULL;
struct thread* thread_current = NULL;

// Prototypes des fonctions internes
int thread_delete_context(struct thread* thread_to_del);
void function(void *(func)(void *), void* funcarg);
int thread_init(struct thread* thread);
int init(void);


// Fonctions
extern struct thread* thread_self(void) {
  if (thread_current == NULL)
    if (init())
      exit(-1);
  return thread_current;
}


int thread_init(struct thread* thread){
  thread->priority = 0;
  thread->isfinished = 0;
  thread->joiner = NULL;
  thread->context = malloc(sizeof(ucontext_t));
  if (thread->context == NULL)
    return -1;
  if (getcontext(thread->context))
    return -1;
  thread->context->uc_stack.ss_size = 64*1024;
  thread->context->uc_stack.ss_sp   = malloc(thread->context->uc_stack.ss_size);
  if (thread->context->uc_stack.ss_sp == NULL)
    return -1;
  //#ifdef MODE_DEBUG 
  //thread->valgrind_stackid VALGRIND_STACK_REGISTER(thread->context.uc_stack.ss_sp, thread->context.uc_stack.ss_sp + thread->context.uc_stack.ss_size);
  //#endif
  thread->context->uc_link =  NULL; 
  thread->retval = NULL;
  return 0;
}

int init(void){
  // On alloue le thread principal
  struct thread* main_thread = malloc(sizeof(struct thread));
  if (main_thread == NULL)
    return -1;
  if (thread_init(main_thread))
    return -1;
  thread_current = main_thread;
  
  // Puis on crée le thread_set
  thread_set = structure_init();
  if (thread_set == NULL)
    return -1;
  return 0;
}

void function(void *(func)(void *), void* funcarg) {
  void * retour = func(funcarg);
  thread_exit(retour);
}

extern int thread_create(struct thread** new_thread, void *(*func)(void *), void *funcarg) {
  // Si c'est la première fois qu'on crée un thread
  if (thread_current == NULL)
    if (init())
      exit(-1);
       
  *new_thread = malloc(sizeof(struct thread));
  if (*new_thread == NULL)
    return -1;
  if (thread_init(*new_thread))
    return -1;
  if (structure_add(thread_set,(*new_thread)))
    return -1;
  
  makecontext((*new_thread)->context,
	      (void (*)(void)) function,
	      2, (int)func, (int)funcarg);
  return 0;
}


// pour le moment sans priorité
extern int thread_yield (void) {
  // Sauvegarder le contexte courant, charger le suivant et changer le current
  struct thread* tmp = structure_get(thread_set);
  struct thread* current;
  int i;
  if (tmp != NULL){
    current = thread_current;
    if (structure_add(thread_set, thread_current))
      return -1;
    thread_current = tmp; 
    
    i = 0;
    if (!current->isfinished)
      if (getcontext(current->context))
	return -1;
    if (i==0){
      i++;
      if (setcontext(thread_current->context))
	return -1;
    }
    return 0;
  }
  return -1;
}


extern int thread_join(struct thread* thread, void **retval){
  struct thread* current = thread_current;  
  struct thread* tmp = NULL;
  int i = 0;
  // Si l'on a jamais appelé thread_create
  if (thread_current == NULL)
    exit(-1);

  //structure_print(thread_set);
  
  // Si le thread n'est pas déjà fini
  if (!thread->isfinished) {
    thread->joiner = thread_current;
    // passer la main
    tmp = structure_get(thread_set);
    if (tmp == NULL){
      fprintf(stderr, "%s l.%d:\t%s\n", __FILE__, __LINE__, "Problème si l'on attends un thread qui n'existe pas");
      exit(-1);
    }
    thread_current = tmp; 
    i = 0;
    if (getcontext(current->context))
      return -1;
    if ( i == 0 ){
      i = 1;
      if (setcontext(thread_current->context))
	return -1;
    } 
  }
  
  // Ici on a repris la main
  if (retval != NULL)
    *retval = thread->retval;
  if (thread_delete(thread))
    return -1;
  
  return 0; 
}


extern void thread_exit(void *retval) { 
  // Si l'on a jamais appelé thread_create
  if (thread_set == NULL)
    exit(0);
  
  struct thread* current = thread_current; 
  thread_current->isfinished = 1;
  thread_current->retval = retval;   
 
  // Si on l'attend on passe la main à celui qui attend, sinon à un autre
  if (current->joiner != NULL){
    thread_current = current->joiner;
  } else {
    thread_current = structure_get(thread_set);
  }
  // Si il n'y a plus de thread à exécuter
  if ( thread_current == NULL)
    exit(0);
  setcontext(thread_current->context);    
  exit(-1);
}


int thread_delete(struct thread* thread_to_del){
  if (thread_to_del != NULL){
    thread_delete_context(thread_to_del);
    free(thread_to_del);
    return 0;
  }
  return -1;
}

int thread_delete_context(struct thread* thread_to_del){
  if (thread_to_del != NULL){
    if (thread_to_del->context != NULL){
      //#ifdef MODE_DEBUG
      //VALGRIND_STACK_DEREGISTER(thread_to_del->valgrind_stackid);
      //#endif
      free(thread_to_del->context->uc_stack.ss_sp);
      free(thread_to_del->context);
      thread_to_del->context = NULL;
      return 0;
    }     
  }
  return -1;
}


int thread_isfinished(struct thread* thread){
  if (thread_current == NULL)
    if (init())
      exit(-1);
  return thread->isfinished;
}

int thread_getpriority(struct thread* thread){
  if (thread_current == NULL)
    if (init())
      exit(-1);
  return thread->priority;
}

