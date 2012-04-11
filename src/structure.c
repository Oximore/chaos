#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "thread.h"

struct data * data_init()
{
  struct data * d = malloc(sizeof(struct data));
  if(d==NULL)
    return NULL;
  d->list = list_init();
  d->tree = tree_init();
  if(d->list==NULL||d->tree==NULL)
    return NULL;
  return d;
}

int data_delete(struct data * d)
{
  if(d==NULL)
    return -1;
  if(list_delete(d->list))
    return -1;
  if(tree_delete(d->tree))
    return -1;
  free(d);
  return 0;
}

int data_add(struct data * d, thread_t t)
{
  if(d == NULL || d->list == NULL||d->tree == NULL|| t==NULL)
    return -1;
  if(t->isfinished == 1)//isfinished(t) sera à privilégier dés implémentation
    return list_add(d->list,t);
  else
    return tree_add(d->tree,t);
}

thread_t get_lower_priority_thread(struct data * d)
{
  if(d==NULL||d->tree==NULL)
    return NULL;
  return tree_get(d->tree);
}

int thread_finished_delete(struct data * d, thread_t t)
{
  if(d == NULL||t==NULL)
    return -1;
  return list_thread_delete(d->list,t);
}

struct list * list_init()
{
  struct list * l = malloc(sizeof(struct list));
  if(l==NULL)
    return NULL;
  l->first =NULL;
  return l;
}

struct tree * tree_init()
{
  struct tree * t = malloc(sizeof(struct tree));
  if(t==NULL)
    return NULL;
  t->root =NULL;
  return t;
}

struct node * node_init(thread_t t)
{
  struct node * n = malloc(sizeof(struct node));
  if(n==NULL||t==NULL)
    return NULL;
  n->left =NULL;
  n->right =NULL;
  n->thread = t;
  return n;
}

int list_delete(struct list * l)
{
  if(l==NULL)
    return -1;
  struct node * n = l->first;
  while(n!=NULL)
    {
      n = n->left;
      if(node_thread_delete(l->first))
	return -1;
      l->first = n;
    }
  return 0;
}

int tree_delete(struct tree * t)
{
  if(t==NULL)
    return -1;
  if(node_thread_delete(t->root))
    return -1;
  free(t);
  return 0;
}

int node_thread_delete(struct node * n)
{
  if(n==NULL)
    return -1;
  if(!isleaf(n))
    {
      if(node_thread_delete(n->left))
	return -1;
      if(node_thread_delete(n->right))
	return -1;
    }
  if(thread_delete(n->thread))
    return -1;
  free(n);
  return 0;
}

int list_add(struct list * l, thread_t t)
{
  if(l==NULL||t==NULL)
    return -1;
  struct node * n = node_init(t);
  if(n==NULL)
    return -1;
  n->left = l->first;
  l->first = n;
  return 0;
}

int tree_add(struct tree * a, thread_t t)
{
  if(a==NULL||t==NULL)
    return -1;
  struct node * n = node_init(t);
  if(n==NULL)
    return -1;
  tree_add_rec(a->root,n);
  balancing(a);
  return 0;
}

void tree_add_rec(struct node * n, struct node * n2)
{
  if((n2->thread->priority) > (n->thread->priority))
    {
      if(n->right == NULL)
	{
	  n->right = n2;
	  return;
	}
      tree_add_rec(n->right,n2);
    }
  else
    {
      if(n->left == NULL)
	{
	  n->left = n2;
	  return;
	}
      tree_add_rec(n->left,n2);
    }
}

int list_thread_delete(struct list * l,thread_t t)
{
  if(l==NULL||t==NULL||l->first==NULL)
    return -1;
  if(l->first->left==NULL)
    {
      if(l->first->thread==t)
	{
	  if(node_thread_delete(l->first))
	    return -1;
	  l->first=NULL;
	}
      return 0;
    }
  struct node * n = l->first;
  if(n->thread==t)
    {
      l->first = l->first->left;
      n->left = NULL;
      n->right = NULL;
      return node_thread_delete(n);
    }
  while(n->left!=NULL&&n->left->thread!=t)
      n=n->left;
  if(n==NULL)
    return -1;
  struct node * p = n->left;
  n->left = n->left->left;
  p->left =NULL;
  p->right =NULL;
  return node_thread_delete(p);
}


// en fait petit probleme ici il ne faut pas prendre la racine comme j'ai dit, il faut prendre le min (ce n'est pas la racine, c'est le + a gauche)
thread_t tree_get(struct tree * t){
  if(t==NULL)
    return NULL;
  thread_t root_value=t->root->thread;
  if(t->root->left==NULL)
    t->root=t->root->right;
  else if(t->root->right==NULL)
    t->root=t->root->left;
  else{
    struct node* cur=t->root->left;
    struct node tmp;
    while(cur->right!=NULL)
      cur=cur->right;
    t->root->thread=cur->thread;
    if(cur->left!=NULL){
      tmp=*cur->left;
      cur=&tmp;
      node_thread_delete(cur->left);
    }
  }
  balancing(t);
  return root_value;
}


thread_t node_delete(struct node * n)
{
  if(n==NULL)
    return NULL;
  thread_t t = n->thread;
  free(n);
  return t;
}

int isleaf(struct node * n)
{
  return (n->left==NULL&&n->right==NULL);
}

int max(int a, int b)
{
  return ((a>=b)?a:b);

}

int height(struct node* t){
  if(t==NULL)
    return 0;
  else
    return 1+max(height(t->left),height(t->right));
}

struct node* rot_left(struct node* n){
  struct tree* b=tree_init();
  b->root=n->right;
  n->right=b->root->left;
  b->root->left=n;
  
  return b->root;

}

struct node* rot_right(struct node* n){
  struct tree* b=tree_init();
  b->root=n->left;
  n->left=b->root->right;
  b->root->right=n;
  return b->root;
}

void balancing(struct tree* t){
  int h_left=height(t->root->left);
  int h_right=height(t->root->right);
  if((h_left-h_right)==2){
    if(height(t->root->left->left) <height(t->root->left->right))
      t->root->left=rot_left(t->root->left);
    t->root=rot_right(t->root);
  }
  if((h_left-h_right)==-2){
    if(height(t->root->right->right) <height(t->root->right->left))
      t->root->right=rot_right(t->root->right);
    t->root=rot_left(t->root);
  }
}


