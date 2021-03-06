#include "list.h"
#include "thread.h"
#include "thread_tools.h"

struct list * list_init()
{
  struct list * l = malloc(sizeof(struct list));
  l->first = NULL;
  l->last = NULL;
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

int list_add(struct list * l, thread_t t)
{
  if(l->first ==NULL)
    {
      struct element * e = element_init(t);
      l->last = e;
      l->first = e;
      l->size++;
    }
  else
    {
      struct element * e = element_init(t);
      e->next = l->first;
      l->first = e;
      l->size++;
    }
  return 0;
}

int list_add_last(struct list * l, thread_t t)
{
  if(l->first ==NULL)
    {
      struct element * e = element_init(t);
      l->last = e;
      l->first = e;
      l->size++;
    }
  else
    {
      struct element * e = element_init(t);
      l->last->next = e;
      l->last = e;
      l->size++;
    }
  return 0;
}

int data_add(struct list * l, thread_t t){
  return list_add_last(l,t);
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

void list_element_delete(struct list * l, struct thread * t)
{
  if(l->first == NULL)
    return;
  if(l->first->next ==NULL)
    {
      if(l->first->thread==t)
	{
	  free(l->first);
	  l->first = NULL;
	  l->size =0;
	  return;
	}
      return;
    }
  if(l->first->thread ==t)
    {
      free(l->first);
      l->first=NULL;
      l->size--;
      return;
    }
  struct element * tmp = l->first;
  while(tmp->next != NULL)
    {
      if(tmp->next->thread==t)
	{
	  if(tmp->next == l->last)
	    {
	      free(tmp->next);
	      tmp->next = NULL;
	      l->size--;
	      l->last = tmp;
	      return;
	    }
	  else
	    {
	      struct element * e = tmp->next->next;
	      free(tmp->next);
	      tmp->next = e;
	      l->size--;
	      return;
	    }
	}
      tmp = tmp->next;
    }
}


void print_list(struct list * l)
{
  printf("liste :\t");
  if(l==NULL)
    return;
  struct element * e = l->first;
  while(e!=NULL)
    {
      printf("%p(%d)\t",e->thread,thread_isfinished(e->thread));//e->thread->isfinished);
      e=e->next;
    }
  printf("\n");
}

thread_t get_lower_priority_thread(struct list * l)
{
  //print_list(l);
  struct element * e; 
  if(l == NULL || l->first == NULL)
    return NULL;

  if(thread_isfinished(l->first->thread))
    {
      struct element * f;
      e = l->first;
      if(e->next == NULL)
	return NULL;
      while(e->next != NULL || thread_isfinished(e->next->thread))
	e = e->next;
      if(e->next==NULL)
	return NULL;
      f = e->next;
      e->next = e->next->next;
      if (e->next == NULL)
	l->last = e;
      l->size--;
      return f->thread;
    }
  
  if(l->first->next == NULL)
    {
      e = l->first;
      l->first = l->first->next;
      l->size--;
      l->last = NULL;
    }
  else
    {
      e = l->first;
      l->first = l->first->next;
      l->size--;
    }
  return e->thread;
}


int is_empty(struct list * l)
{
  if(l==NULL)
    return -1;
  if(l->first==NULL)
    return 1;
  return 0;
}
