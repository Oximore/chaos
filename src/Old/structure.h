#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#include <ucontext.h>
#include "thread_tools.h"

struct data
{
  struct list * list;
  struct tree * tree;
};

struct list 
{
  struct node * first;
};

struct tree
{
  struct node * root;
};

struct node
{
  struct node * left;
  struct node * right;
  struct node * root;
  thread_t thread; 
};

// les delete renvoient une erreur si on tente de supprimer un thread dans une structure vide (mais bien initialisée)

//modifier les fct pour ret erreur
//init fait
//add fait
//delete fait
struct data * data_init();
int data_delete(struct data * );
int data_add(struct data *, thread_t );
thread_t get_lower_priority_thread(struct data *);
//thread_t get_lower_priority_thread(struct data *); sans le suppr
int thread_finished_delete(struct data *, thread_t);

//int data_create(list ** pointeur);

struct list * list_init();
struct tree * tree_init();
struct node * node_init(thread_t);
int list_delete(struct list * );
int tree_delete(struct tree * );
int list_add(struct list *, thread_t );
int tree_add(struct tree *, thread_t ); // ne garde pas l'arbre équilibré !
void tree_add_rec(struct node * , struct node * );

int list_thread_delete(struct list *,thread_t);
thread_t tree_get(struct tree *);

int node_thread_delete(struct node * );
thread_t node_delete(struct node * );

int isleaf(struct node *);

int height(struct node*);
struct node* rot_right(struct node*);
struct node* rot_left(struct node*);
void balancing(struct node**);//équilibrage
void balance(struct node**);

void print_data(struct data* d);
void print_tree(struct node* n, int p);



#endif /* __STRUCTURE_H__ */
