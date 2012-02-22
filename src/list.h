#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

struct list 
{
  struct element * first;
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
void list_element_delete(struct list *, thread_t );
struct element * element_init(thread_t );
struct element * element_delete(struct element * );

#endif /* __LIST_H__ */
