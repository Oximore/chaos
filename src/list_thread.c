#include <stdio.h>
#include <stdlib.h>
#include "list_thread.h"

/*** struct list ***/

struct list* list_init(void) {
  struct list* l = malloc(sizeof(struct list));
  if (l != NULL) {
    l->first = NULL;
    l->last = NULL;
  }
  return l;
}

// free thread
int list_delete(struct list* l) {
  struct element* e,* f;
  if (l != NULL) {
    e = l->first;
    while (e != NULL){
      f = e->next;
      thread_delete(e->thread);
      free(e);
      e = f;
    }
    free(l);
  }
  return 0;
}

// add in first
int list_add(struct list* l, thread_t t) {
  if (l == NULL || t == NULL) {
    return -1;
  }
  if (!thread_isfinished(t)) {
    struct element* e = element_new(t);
    if (e == NULL)
      return -1;
    
    if (l->first == NULL) {
      l->last = e;
    } else {
      l->first->previous = e;
    }
    e->next = l->first;
    l->first = e;
  }
  return 0;
}

// get in last
thread_t list_get(struct list* l) {
  struct element* e;
  thread_t t;
  if (l == NULL || l->last == NULL)
    return NULL;
  
  if (l->last->previous != NULL) {
    l->last->previous->next = NULL;
  } else {
    l->first = NULL;  
  }
  
  e = l->last;
  l->last = l->last->previous;
  t = e->thread;
  element_delete(e);
  
  return t;
}




/*** struct element ***/
struct element* element_new(thread_t t) {
  struct element* e = malloc(sizeof(struct element));
  if (e != NULL) {
    e->previous = NULL;
    e->next     = NULL;
    e->thread   = t;
  }
  return e;
}

// don't free the thread_t structure
int element_delete(struct element* e) {
  free(e);
  return 0;
}

/*** debug functions ***/
void list_print(struct list* l) {
  fprint_list(stderr,l);
}

void fprint_list(FILE* f, struct list* l) {
  if (l == NULL)
    fprintf(f,"not init list");
  else if (l->first == NULL) 
    fprintf(f,"empty list");
  else {
    struct element* e;
    e = l->first;
    while (e != NULL) {
      fprintf(f,"%d\t",thread_getpriority(e->thread));
      e = e->next;
    }
  }
  fprintf(f,"\n");  
}
