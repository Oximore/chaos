#include <stdio.h>
#include <stdlib.h>
#include "thread_mix.h"
#include "time.h"

int affiche_arg = 0;

void* boucle(void* arg);

int main(int argc, char* argv[]) {
  // bricolage pour un make sans warning
  affiche_arg = (int) argv[0];
  if (argc>1)
    affiche_arg = 1;
  else
    affiche_arg = 0;

  printf("Création/Destruction de thread de manière infinie.\n");
  time_start(); {
    mthread_t thread;
    mthread_create(&thread, boucle, (void*)mthread_self());
    mthread_exit((void*) 1);
  }
  time_end();
  return EXIT_SUCCESS;
}

void* boucle(void* arg) {
  unsigned int n;
  mthread_t thread;
  // on attend la fin du précédent
  mthread_join((mthread_t) arg, (void *) &n);
  // on crée le suivant
  mthread_create(&thread, boucle, (void*)mthread_self());
  if (affiche_arg)
    printf("%d\t", n);
  return (void*)(n+1);
}
