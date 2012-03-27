#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include "thread.h"


struct list 
{
  struct element * first;
  struct element * last;
  int size;
};

struct element
{
  struct element * next;
  thread_t thread;
};

struct list * list_init();
void list_delete(struct list * );
void list_add(struct list *, thread_t );
void list_add_last(struct list *, thread_t );
void list_element_delete(struct list *,struct element *, int(*fct)(struct element *,struct element *));
struct element * element_init(thread_t );
struct element * element_delete(struct element * );
void print_list(struct list * );
thread_t get_lower_priority_thread(struct list *);
int is_empty(struct list * );
#endif /* __LIST_H__ */
