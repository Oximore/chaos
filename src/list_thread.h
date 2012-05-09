#ifndef __LIST_THREAD_H__
#define __LIST_THREAD_H__

#include <stdio.h>
#include <stdlib.h>
#include "thread_tools.h"

struct list 
{
  struct element * first;
  struct element * last;
};

struct element
{
  struct element * next;
  thread_t thread;
};

struct list * list_init();
int list_delete(struct list * );
int list_add(struct list *, thread_t );
thread_t list_get(struct list * );
struct element * element_init(thread_t );
struct element * element_delete(struct element * );

void fprint_list(FILE* f, struct list* l);
void list_print(struct list* l);

#endif /* __LIST_THREAD_H__ */
