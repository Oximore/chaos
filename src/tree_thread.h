#ifndef __TREE_THREAD_H__
#define __TREE_THREAD_H__

#include "thread_tools.h"


struct tree {
  struct node * root;
};

struct node {
  struct node * left;
  struct node * right;
  struct node * root;
  thread_t thread; 
};

// les delete renvoient une erreur si on tente de supprimer un thread dans une structure vide (mais bien initialisée)


struct tree* tree_init   ();
int          tree_delete (struct tree *);
int          tree_add    (struct tree *, thread_t );
void         tree_print  (struct tree *);


void print_node(struct node* n, int p);


struct node * node_init(thread_t);
void tree_add_rec(struct node * , struct node * );
thread_t tree_get(struct tree *);

int node_thread_delete(struct node * );
thread_t node_delete(struct node * );

int isleaf(struct node *);

int height(struct node*);
struct node* rot_right(struct node*);
struct node* rot_left(struct node*);
void balancing(struct node**);//équilibrage
void balance(struct node**);

#endif /* __TREE_THREAD_H__ */
