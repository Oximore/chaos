#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#ifdef MODE_LIST 

#include "list_thread.h"
typedef struct list* structure;

#endif

#ifndef MODE_LIST

#include "tree_thread.h"
typedef struct tree* structure;

#endif

structure structure_init ();
int       structure_add  (structure, thread_t);
void      structure_print(structure);
thread_t  structure_get  (structure);
#endif
