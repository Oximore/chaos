#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "thread.h"

struct data * data_init()
{
  struct data * d = malloc(sizeof(struct data));
  d->list = list_init();
  d->tree = tree_init();
  return d;
}

void data_delete(struct data * d)
{
  list_delete(d->list);
  tree_delete(d->tree);
  free(d);
}

void data_add(struct data * d, thread_t t)
{
  if(d == NULL || d->list == NULL||d->tree == NULL|| t==NULL)
    return NULL;
  if(isfinished(t))
    list_add(d->list,t);
  else
    tree_add(d->tree,t);
}

thread_t get_lower_priority_thread(struct data * d)
{
  return tree_get(d->tree);
}

void thread_finished_delete(struct data * d, thread_t t)
{
  list_thread_delete(d->list,t);
}

struct list * list_init()
{
  struct list * l = malloc(sizeof(struct list));
  l->first =NULL;
  return l;
}

struct tree * tree_init()
{
  struct tree * t = malloc(sizeof(struct tree));
  t->root =NULL;
  return t;
}

struct node * node_init(thread_t t)
{
  struct node * n = malloc(sizeof(struct node));
  n->left =NULL;
  n->right =NULL;
  n->thread = t;
  return n;
}

void list_delete(struct list * l)
{
  if(l==NULL)
    return;
  struct node * n = l->first;
  while(n!=NULL)
    {
      n = n->left;
      node_thread_delete(l->first);
      l->first = n;
    }
}

void tree_delete(struct tree * t)
{
  node_thread_delete(t->root);
}

void node_thread_delete(struct node * n)
{
  if(n==NULL)
    return;
  if(!isleaf(n))
    {
      node_thread_delete(n->left);
      node_thread_delete(n->right);
    }
  thread_delete(n->thread);
  free(n);
}

void list_add(struct list * l, thread_t t)
{
  if(l==NULL||t==NULL)
    return;
  struct node * n = node-init(t);
  n->left = l->first;
  l->first = n;
}

void tree_add(struct tree *, thread_t );

void list_thread_delete(struct list *,thread_t);
thread_t tree_get(struct tree *);


thread_t node_delete(struct node * n)
{
  thread_t t = n->thread;
  free(n);
  return t;
}



