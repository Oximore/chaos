 #include "list_thread.h"

struct list * list_init() {
  struct list * l = malloc(sizeof(struct list));
  if (l != NULL) {
    l->first = NULL;
    l->last = NULL;
  }
  return l;
}

int list_delete(struct list * l) {
  if(l == NULL)
    return -1;
  
  struct element * e = l->first;
  while (e != NULL)
    e = element_delete(e);
      
  free(l);
  return 0;
}



int list_add(struct list * l, thread_t t) {
  if( !thread_isfinished(t) ) {
    struct element* e = element_init(t);
    if (l->first == NULL) {
      l->first = e;
    } else {
      l->last->next = e;
    }
    l->last = e;
  }
  return 0;
}

thread_t list_get(struct list * l) {
  struct element * e;
  thread_t t;
  if (l == NULL || l->first == NULL)
    return NULL;
  e = l->first;
  t = e->thread;
  if (l->first->next == NULL) {
    l->last = NULL;
  }
  l->first = l->first->next;
  free(e);
  return t;
}


struct element * element_init(thread_t t) {
  struct element * e = malloc(sizeof(struct element));
  e->next = NULL;
  e->thread = t;
  return e;
}

struct element * element_delete(struct element * e) {
  struct element * tmp = e->next;
  thread_delete(e->thread);
  free(e);
  return tmp;
}



/*** debug functions ***/
void list_print(struct list* l) {
  fprint_list(stderr,l);
}

void fprint_list(FILE* f, struct list* l) {
  int i = 0;
  if (l == NULL)
    fprintf(f,"not init list");
  //  else if (l->first == NULL) 
  //  fprintf(f,"empty list");
  else {
    struct element* e;
    e = l->first;
    while (e != NULL) {
      //      fprintf(f,"%d\t",thread_getpriority(e->thread));
      i++;
      e = e->next;
    }
  }
  fprintf(f,"%d\t", i);  
}
