#ifndef __LIST_THREAD_H__
#define __LIST_THREAD_H__

#include <stdio.h>
#include "thread_tools.h"

struct list {
  struct element* first;
  struct element* last;
};

struct element {
  struct element* next;
  struct element* previous;
  thread_t thread;
};


struct list* list_init   (void);
int          list_delete (struct list* l);
int          list_add    (struct list* l, thread_t t);
thread_t     list_get    (struct list* l);

struct element* element_new(thread_t t);
int element_delete(struct element* e);

void fprint_list(FILE* f, struct list* l);
void list_print(struct list* l);

#endif
