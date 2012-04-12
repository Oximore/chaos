// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include "thread_tools.h"
//#include "list.h"
#include "structure.h"
// get_lower_priority_thread() and data_add()

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
  struct thread* joiner;
  int isfinished;
};


// Variables Globales
//struct list* thread_list      = NULL;
struct data* thread_data      = NULL;
struct thread* thread_current = NULL;

// Prototypes des fonctions internes
int thread_delete_context(struct thread* thread_to_del);
void function(void *(func)(void *), void* funcarg);
int thread_init(struct thread* thread);


//
//    *TODO*
// -> gérer les valeurs de retours
// -> gérer les appels aux fonctions pour la 1ere fois
//
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
  if (thread->context == NULL)
    return -1;
  if (getcontext(thread->context)) // utile ici ?
    return -1;
  // Malloc t'on la pile ? si oui (pas par défaut) quelle taille ?
  thread->context->uc_stack.ss_size = 64*1024;
  thread->context->uc_stack.ss_sp   = malloc(thread->context->uc_stack.ss_size);
  if (thread->context->uc_stack.ss_sp == NULL)
    return -1;
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
    if (main_thread == NULL)
      return -1;
    if (thread_init(main_thread))
      return -1;
    thread_current = main_thread;
    // Puis on crée la thread_data
    //thread_list = list_init(); 
    thread_data = data_init();
    if (thread_data == NULL)
      return -1;
  }
  
  *new_thread = malloc(sizeof(struct thread));
  if (*new_thread == NULL)
    return -1;
  if (thread_init(*new_thread))
    return -1;
  if (data_add(thread_data,(*new_thread)))
    return -1;
  
  makecontext((*new_thread)->context,
	      (void (*)(void)) function,
	      2, (int)func, (int)funcarg);
  return 0;
}


// pour le moment sans priorité
extern int thread_yield (void) {
  debug("thread_yield");
  // Sauvegarder le contexte courant, charger le suivant et changer le current
  struct thread* tmp = get_lower_priority_thread(thread_data);
  struct thread* current;
  int i;
  if (tmp != NULL){
    current = thread_current;
    if (data_add(thread_data, thread_current))
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
  debug("thread_join");
  struct thread* current = thread_current;  
  struct thread* tmp = NULL;
  int i = 0;
  // Si l'on a jamais appelé thread_create
  if (thread_current == NULL)
    exit(2); // *TODO*   exit(-1) à la fin
  //print_data(thread_data);

  // Si le thread n'est pas déjà fini
  if (!thread->isfinished){
    thread->joiner = thread_current;
    // passer la main
    tmp = get_lower_priority_thread(thread_data);
    if (tmp == NULL){
      debug("problème si on attend un thread qui n'existe pas .."); 
      return -1;
      //exit(42);
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
  
  // Pour le moment reste dans la pile si est join plus tard


  // *TODO* à décommenter plus tard
  //if (thread_finished_delete(thread_data,thread))
  //  return -1;
  
  return 0; 
}


//extern void thread_exit(void *retval) __attribute__ ((__noreturn__)){ 
extern void thread_exit(void *retval){ 
  debug("thread_exit");
  // Si l'on a jamais appelé thread_create
  if (thread_data == NULL)
    exit(0);
  
  struct thread* current = thread_current; 
  thread_current->isfinished = 1;
  thread_current->retval = retval;   
  // thread_delete_context(thread_current);
 
  // Si on l'attend on passe la main à celui qui attend, sinon à un autre
  if (current->joiner != NULL){
    thread_current = current->joiner;
  } else {
    thread_current = get_lower_priority_thread(thread_data);
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
    return 0;
  }
  return -1;
}

int thread_delete_context(struct thread* thread_to_del){
  debug("thread_delete_context");
  if (thread_to_del != NULL){
    if (thread_to_del->context != NULL){
      //if (thread_to_del->context->uc_stack.ss_sp != NULL){ }
      free(thread_to_del->context->uc_stack.ss_sp);
      free(thread_to_del->context);
      thread_to_del->context = NULL;
      return 0;
    }     
  }
  return -1;
}


int thread_isfinished(struct thread* thread){
  debug("thread_isfinished");
  return thread->isfinished;
}

int thread_getpriority(struct thread* thread){
  debug("thread_getpriority");
  return thread->priority;
}
