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
  //  int isjoined; // == (joiner != NULL)
  struct thread* joiner;
  int isfinished;
};


// Variables Globales
struct list* thread_list      = NULL;
struct thread* thread_current = NULL;

// Prototypes des fonctions internes
int thread_delete_context(struct thread* thread_to_del);
void function(void *(func)(void *), void* funcarg);
int thread_init(struct thread* thread);

// Fonctions
extern struct thread* thread_self(void) {
  debug("thread_self");
  return thread_current;
}


int thread_init(struct thread* thread){
  debug("thread_init");
  thread->priority = 0;
  thread->isfinished = 0;
  thread->joiner = NULL;
  thread->context = malloc(sizeof(ucontext_t));
  getcontext(thread->context); // utile ici ?
  // Malloc t'on la pile ? si oui (pas par défaut) quelle taille ?
  thread->context->uc_stack.ss_size = 64*1024;
  thread->context->uc_stack.ss_sp   = malloc(thread->context->uc_stack.ss_size);
  thread->context->uc_link =  NULL; 
  thread->retval = NULL;
  return 0;
}

void function(void *(func)(void *), void* funcarg) {
  debug("function");
  void * retour = func(funcarg);
  thread_exit(retour);
}

extern int thread_create(struct thread** new_thread, void *(*func)(void *), void *funcarg) {
  debug("thread_create");
  // Si c'est la première fois qu'on crée un thread
  if (thread_current == NULL) {
    // On alloue le thread principal
    struct thread* main_thread = malloc(sizeof(struct thread));
    thread_init(main_thread);
    thread_current = main_thread;
    // Puis on crée la thread_list
    thread_list = list_init(); 
  }
  
  *new_thread = malloc(sizeof(struct thread));
  thread_init(*new_thread);
  list_add_last(thread_list,(*new_thread));
  makecontext((*new_thread)->context,
	      (void (*)(void)) function,
	      2, (int)func, (int)funcarg);
  return 0;  // *TODO* valeur de retour
}


// pour le moment sans priorité
extern int thread_yield (void) {
  debug("thread_yield");
  // Sauvegarder le contexte courant, charger le suivant et changer le current
  struct thread* tmp = get_lower_priority_thread(thread_list);
  struct thread* current;
  int i;
  if (tmp != NULL){
    current = thread_current;
    list_add_last(thread_list, thread_current);
    thread_current = tmp; 
    
    i = 0;
    if (!current->isfinished)
      getcontext(current->context);
    if (i==0){
      i++;
      setcontext(thread_current->context);
    }
    return 0;
  }
  return -1;
}


extern int thread_join(struct thread* thread, void **retval){
  debug("thread_join");
  struct thread* current = thread_current;  
  struct thread* tmp = NULL;
  int i = 0;
  
  // Si le thread n'est pas déjà fini
  if (!thread->isfinished){
    thread->joiner = thread_current;
    // passer la main
    tmp = get_lower_priority_thread(thread_list);
    if (tmp == NULL){
      debug("problème si on attend un thread qui n'existe pas .."); 
      exit(1);
    }
    thread_current = tmp; 
    i = 0;
    getcontext(current->context);
    //debug("une boucle infine ?");
    if ( i == 0 ){
      i = 1;
      setcontext(thread_current->context);
    }    
  }

  // Ici on a repris la main
  if (retval != NULL)
    *retval = thread->retval;
  thread_delete(thread);
  return 0; // *TODO*
}


//extern void thread_exit(void *retval) __attribute__ ((__noreturn__)){ 
extern void thread_exit(void *retval){ 
  debug("thread_exit");
  // Si l'on a jamais appelé thread_create
  if (thread_list == NULL)
    exit(0);
  
  struct thread* current = thread_current; 
  thread_current->isfinished = 1;
  thread_current->retval = retval;   
  thread_delete_context(thread_current);
 
  // Si on l'attend on passe la main à celui qui attend, sinon à un autre
  if (current->joiner != NULL){
    thread_current = current->joiner;
  } else {
    thread_current = get_lower_priority_thread(thread_list);
  }
  // Si il n'y a plus de thread à exécuter
  if ( thread_current == NULL)
    exit(0);
  setcontext(thread_current->context);    
}


int thread_delete(struct thread* thread_to_del){
  debug("thread_delete");
  if (thread_to_del != NULL){
    thread_delete_context(thread_to_del);
    free(thread_to_del);
    return 1;
  }
  else return 1;
}

int thread_delete_context(struct thread* thread_to_del){
  debug("thread_delete_context");
  if (thread_to_del != NULL){
    if (thread_to_del->context != NULL){
      if (thread_to_del->context->uc_stack.ss_sp != NULL){
	free(thread_to_del->context->uc_stack.ss_sp);
      }
      free(thread_to_del->context);
      thread_to_del->context = NULL;
      return 0;
    }     
  }
  return 1;
}


int thread_isfinished(struct thread* thread){
  debug("thread_isfinished");
  return thread->isfinished;
}

int thread_getpriority(struct thread* thread){
  debug("thread_getpriority");
  return thread->priority;
}
