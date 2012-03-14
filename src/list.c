#include "list.h"

struct list * list_init()
{
  struct list * l = malloc(sizeof(struct list));
  l->first = NULL;
  l->size = 0;
  return l;
}

void list_delete(struct list * l)
{
  if(l ==NULL)
    return;
  if(l->first == NULL)
    {
      free(l);
      return;
    }
  if(l->first->next == NULL)
    {
      element_delete(l->first->next);
      free(l);
      return;
    }
  struct element * tmp = l->first;
  while(tmp->next != NULL)
    {
      tmp = tmp->next;
      element_delete(l->first);
      l->first = tmp;
    }
  free(l);
}

void list_add(struct list * l, thread_t t)
{
  struct element * e = element_init(t);
  e->next = l->first;
  l->first = e;
  l->size++;
}


struct element * element_init(thread_t t)
{
  struct element * e = malloc(sizeof(struct element));
  e->next = NULL;
  e->thread = t;
  return e;
}

struct element * element_delete(struct element * e)
{
  struct element * tmp = e->next;
  thread_delete(e->thread);
  free(e);
  return tmp;
}

void list_element_delete(struct list * l, struct element * e, int(*fct)(struct element *,struct element *))
{
  if(l->first == NULL)
    return;
  if(l->first->next ==NULL)
    {
      if(fct(l->first,e))
	{
	  element_delete(l->first);
	  l->size =0;
	  return;
	}
      return;
    }
  if(fct(l->first,e))
    {
      element_delete(l->first);
      l->size--;
      return;
    }
  struct element * tmp = l->first;
  while(tmp->next != NULL)
    {
      if(fct(tmp->next,e))
	{
	  struct element * e = element_delete(tmp->next);
	  tmp->next = e;
	  l->size--;
	  return;
	}
      tmp = tmp->next;
    }
}

void print_list(struct list * l)
{
  struct element * e = l->first;
  while(e!=NULL)
    {
      printf("%d\n",e->thread->priority);
      e=e->next;
    }
}
