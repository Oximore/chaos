#include <stdio.h>
#include <stdlib.h>
#include "tree_thread.h"


struct tree * tree_init() {
  struct tree * t = malloc(sizeof(struct tree));
  if (t == NULL)
    return NULL;
  t->root = NULL;
  return t;
}

struct node * node_init(thread_t t) {
  struct node * n = malloc(sizeof(struct node));
  if(n==NULL||t==NULL)
    return NULL;
  n->left =NULL;
  n->right =NULL;
  n->root = NULL;
  n->thread = t;
  return n;
}

int tree_delete(struct tree * t) {
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

int tree_add(struct tree * a, thread_t t)
{
  if(a==NULL||t==NULL)
    return -1;
  struct node * n = node_init(t);
  if(n==NULL)
    return -1;
  if(a->root == NULL)
    {
      a->root = n;
      return 0;
    }
  tree_add_rec(a->root,n);
  return 0;
}

void tree_add_rec(struct node * n, struct node * n2)
{
  if((thread_getpriority(n2->thread)) > (thread_getpriority(n->thread)))
    {
      if(n->right == NULL)
	{
	  n->right = n2;
	  n2->root = n;
	  balancing(&n);
	  return;
	}
      tree_add_rec(n->right,n2);
    }
  else
    {
      if(n->left == NULL)
	{
	  n->left = n2;
	  n2->root = n;
	  balancing(&n);
	  return;
	}
      tree_add_rec(n->left,n2);
    }
}
thread_t tree_get(struct tree * t){
  thread_t thread;
  struct node* node_tmp;
  struct node* node_right_tmp;

  if (t==NULL || t->root==NULL)
    return NULL;
  
  if (t->root->left == NULL)
    {
      node_right_tmp = t->root->right;
      thread = t->root->thread;
      node_delete(t->root);
      // On racroche soit NULL soit le droit
      t->root = node_right_tmp;
      if(t->root!=NULL)
	t->root->root=NULL;
      return thread;
    }
  
  node_tmp = t->root;
  while (node_tmp->left->left != NULL) 
    node_tmp = node_tmp->left;

  thread = node_tmp->left->thread;
  node_right_tmp = node_tmp->left->right;
  node_delete(node_tmp->left);
  // On racroche soit NULL soit le droit
  node_tmp->left = node_right_tmp;
  if(node_tmp->left != NULL)
    node_tmp->left->root = node_tmp;
    
  return thread;
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

struct node* rot_left(struct node* n)
{
  struct node * n_tmp = n->right;
  n->right = n->right->left;
  if(n->right !=NULL)
    n->right->root = n;
  n_tmp->left = n;
  n_tmp->root = n->root;
  n->root = n_tmp;
  return n_tmp;
}


struct node* rot_right(struct node* n)
{
  struct node * n_tmp = n->left;
  n->left = n->left->right;
  if(n->left !=NULL)
    n->left->root = n;
  n_tmp->right = n;
  n_tmp->root = n->root;
  n->root = n_tmp;
  return n_tmp;
}



void balance(struct node** n2){
  struct node * n = *(n2);
  int h_left  = height(n->left);
  int h_right = height(n->right);
  if((h_left-h_right)>=2)
    {
       if(height(n->left->left) < height(n->left->right))
	 n->left=rot_left(n->left);

       if(n->root !=NULL && n->root->left == n)
	 {
	   n->root->left=rot_right(n);
	 }
       else
	 {
	   if(n->root !=NULL && n->root->right == n)
	     n->root->right=rot_right(n);
	 }
    }
  if((h_left-h_right)<=-2){
    if(height(n->right->right) <height(n->right->left))
      n->right=rot_right(n->right);
    
    if(n->root != NULL && n->root->left == n)
      {
	n->root->left=rot_left(n); 
      }
    else
      {
	if(n->root !=NULL && n->root->right == n)
	  n->root->right=rot_right(n);
      }
  }
}


void balancing(struct node** n){
  struct node * n2 = *(n);
  struct node * root = n2->root;
  balance(n);
  if(root !=NULL)
    balancing(&(root));
}

int nb_node(struct node* n){
  if (n == NULL)
    return 0;
  return 1 + nb_node(n->right) + nb_node(n->left); 
}

void tree_print  (struct tree * t){
  if (t ==NULL) {
    printf("arbre non init\n");
  } else if (t->root == NULL) {
    printf("arbre vide\n");
  } else {
    print_node(t->root,0);
  }
}

void print_node(struct node* n, int p) {
  int i;
  for ( i=0 ; i<p ; i++)
    printf("\t");
  if (n != NULL){
    printf("%d\n", thread_getpriority(n->thread));
    print_node(n->left,p+1);
    print_node(n->right,p+1);
  }
  else
    printf("f\n");
}
