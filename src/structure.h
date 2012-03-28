#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

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
  thread_t thread; 
};


struct data * data_init();
void data_delete(struct data * );
void data_add(struct data *, thread_t );
thread_t get_lower_priority_thread(struct data *);
//thread_t get_lower_priority_thread(struct data *); sans le suppr
void thread_finished_delete(struct data *, thread_t);

struct list * list_init();
struct tree * tree_init();
struct node * node_init(thread_t);
void list_delete(struct list * );
void tree_delete(struct tree * );
void list_add(struct list *, thread_t );
void tree_add(struct tree *, thread_t );

void list_thread_delete(struct list *,thread_t);
thread_t tree_get(struct tree *);

void node_thread_delete(struct node * );
thread_t node_delete(struct node * );

int isleaf(struct node *);







#endif /* __STRUCTURE_H__ */
