#include "structure.h"

structure structure_init(void) {
#ifdef MODE_LIST 
  return list_init();
#endif
#ifndef MODE_LIST
  return tree_init();
#endif  
}

int structure_add(structure s, thread_t t) {
#ifdef MODE_LIST 
  return list_add(s,t);
#endif
#ifndef MODE_LIST
  return tree_add(s,t);
#endif  
}

void structure_print(structure s) { 
#ifdef MODE_LIST 
  list_print(s);
#endif
#ifndef MODE_LIST
  tree_print(s);
#endif  
}

thread_t structure_get(structure s) {
#ifdef MODE_LIST
  return list_get(s);
#endif
#ifndef MODE_LIST
  return tree_get(s);
#endif  
}

