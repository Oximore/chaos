// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "list.h"

//#define debug(arg) fprintf(STDERR, "%s : %d %d", arg, __truc__, __machin__);

// Variables Globales
struct list *thread_list = NULL;
struct thread *thread_current = NULL;

//       int getcontext(ucontext_t *ucp);
//       int setcontext(const ucontext_t *ucp);
//       void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
//       int swapcontext(ucontext_t *oucp, ucontext_t *ucp);

int thread_delete(struct thread* thread_to_del);




// Fonctions
extern thread_t thread_self(void) {
  printf("thread self\n");
  return thread_current;
}


void function(void *(func)(void *), void* funcarg) {

  void * retour = func(funcarg);
  thread_current->retval = retour ;
  thread_current->isfinished = 1;
 
  // Si on l'attend on passe la main et le thread_join le détruira
  if ( thread_current->isjoined )
    thread_yield();

  // sinon on le détruit 
  struct thread *tmp = get_lower_priority_thread(thread_list);
  
  if ( tmp == NULL)
    exit(0);
  thread_delete(thread_current);
  thread_current = tmp;
  setcontext(thread_current->context);
}


extern int thread_create(thread_t* new_thread, void *(*func)(void *), void *funcarg) {
  if (thread_current == NULL) {
    // Si c'est la première fois qu'on crée un thread
    // On alloue le thread principal
    printf("1er passage dans thread_create \n");
    struct thread *main_thread = malloc(sizeof(struct thread));
    main_thread->priority = 0;
    main_thread->isfinished = 0;
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
  
  printf("passage dans thread create 2\n");
  *new_thread = malloc(sizeof(struct thread));
  (*new_thread)->priority = 0;
  (*new_thread)->isfinished = 0;
  (*new_thread)->context = malloc(sizeof(ucontext_t));
  getcontext((*new_thread)->context);
  printf("thread create 3\n");
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
  // Sauvegarder le contexte courant, charger le suivant et changer le current
  // printf("thread_yield 1\n");
  thread_t tmp = get_lower_priority_thread(thread_list);
  thread_t current;
  // printf("thread_yield 2\n");
  //if (tmp == NULL )
  //  printf("tmp == NULL\n");
  
  if (tmp != NULL){
    current = thread_current;
    list_add_last(thread_list, thread_current);
    thread_current = tmp; 
    
    //printf("thread_yield 3 : %p\n",thread_current );
    int i = 0;
    getcontext(current->context);
    printf("thread_yield 4 : %p\n",thread_current );
    
    if (i==0){
      i++;
      printf("i = 1 in setcontext \n");
      setcontext(thread_current->context);
    }    
    i = 0;
    return 0;
  }
  return -1;
}


extern int thread_join(thread_t thread, void **retval){
  thread->isjoined = 1;
  // isjoined ++; pour tous les threads ?
  while (!thread->isfinished)
    thread_yield();
  
  if (retval != NULL)
    *retval = thread->retval;
  thread->isjoined = 0;
  //isjoined --; ??
  
  // On supprime ce thread
  list_element_delete(thread_list, thread);
  thread_delete(thread);
  
  return 0; // *TODO*
}

int thread_delete(struct thread* thread_to_del){
  free(thread_to_del->context->uc_stack.ss_sp);
  free(thread_to_del->context);
  free(thread_to_del);
  return 0;
}

//extern void thread_exit(void *retval) __attribute__ ((__noreturn__)){ 
extern void thread_exit(void *retval){ 
  // Si l'on a jamais appelé thread_create
  printf("thread_exit\n");
  if (thread_list == NULL)
    exit(0);
  
  // Sinon, si on l'attend
  thread_current->isfinished = 1;
  thread_current->retval = retval;   
  if (thread_current->isjoined){ 
    thread_yield();
  }
  
  
  struct thread *tmp = get_lower_priority_thread(thread_list);
  // Si il n'y a plus de thread executable, alors c'est la fin
  if ( tmp  == NULL)
    exit(0);
  list_add(thread_list,tmp);
  
  //  free(thread_current->context->uc_stack.ss_sp);
  //free(thread_current->context);
  //thread_current->context = NULL;
    
  thread_yield();  // Dans le thread_yield on fait le getcontext que si !isfinished
  // Sinon, il faut changer avec tmp
  // *TODO* Enlevé cette FU**** duplication de code !!!
  //thread_delete(thread_current);
  //thread_current = tmp;
  //setcontext(thread_current->context);
} 

