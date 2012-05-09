 #include "list_thread.h"

struct list * list_init()
{
  struct list * l = malloc(sizeof(struct list));
  l->first = NULL;
  l->last = NULL;
  return l;
}

int list_delete(struct list * l)
{
  if(l ==NULL)
    return -1;
  if(l->first == NULL)
    {
      free(l);
      return 0;
    }
  if(l->first->next == NULL)
    {
      element_delete(l->first->next);
      free(l);
      return -1;
    }
  struct element * tmp = l->first;
  while(tmp->next != NULL)
    {
      tmp = tmp->next;
      element_delete(l->first);
      l->first = tmp;
    }
  free(l);
  return 0;
}



int list_add(struct list * l, thread_t t)
{
  if(!thread_isfinished(t))
    {
      if(l->first ==NULL)
	{
	  struct element * e = element_init(t);
	  l->last = e;
	  l->first = e;
	}
      else
	{
	  struct element * e = element_init(t);
	  l->last->next = e;
	  l->last = e;
	}
    }
  return 0;
}

thread_t list_get(struct list * l)
{
  struct element * e;
   if (l == NULL || l->first == NULL)
    return NULL;
   if(l->first->next ==NULL)
     {
       e = l->first;
       l->first =NULL;
       l->last = NULL;
       return e->thread;
     }
   e = l->first;
   l->first = l->first->next;
   return e->thread;
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
