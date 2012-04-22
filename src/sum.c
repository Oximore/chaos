#include <stdio.h>
#include <stdlib.h>
#include "thread_mix.h"
#include "time.h"

// Structures
struct indices{
  unsigned int a;
  unsigned int b;
};


// Variables globales
unsigned long int* tableau;
int affiche_tableau = 0;
int affiche_arg = 0;

// Prototypes
void* tab_sum(void* arg);


// Fonctions
int main(int argc, char* argv[])
{
  unsigned int n = 10, i;
  struct indices indice;

  // Arg Filter
  if (argc > 1)
    n = atoi(argv[1]);
  if (argc > 2)
    affiche_tableau = 1;
  if (argc > 3) 
    affiche_arg = 1;

  
  if (n<1) {
    perror("Veuillez entrer un entier positif.");
    return 0;
  }

  // on crée le tableau
  tableau = malloc (n*sizeof(unsigned long int));
  
  printf("\tn = %u\n", n);
  if (affiche_tableau)
    printf("tab :");
  // on l'initialise
  for ( i=0 ; i<n ; i++) {
    tableau[i] = i;
    if (affiche_tableau)
      printf(" %lu", tableau[i]);
  }
  if (affiche_tableau)
    printf("\n");
  
  indice.a = 0;
  indice.b = n-1;
  
  time_start(); {
    tab_sum((void*) &indice);
    if (affiche_arg)
      printf("\n");
    printf("\tLa somme des éléments du tableau est égale à : %lu\n", tableau[0]);
  }
  time_end();
return EXIT_SUCCESS;	      
}


void* tab_sum(void* arg) {
  struct indices* indice = (struct indices*) arg;
  unsigned int a, b, m;
  a = indice->a;
  b = indice->b;
  
  if (b<=a) {
    if (affiche_arg)
      printf("%u:%u %lu\t",a,b, tableau[a]);
    return NULL; 
  }

  m = (b+a)/2;
  
  mthread_t th1, th2;
  struct indices i1;
  struct indices i2;
  i1.a = a;
  i1.b = m;
  i2.a = m+1;
  i2.b = b;

  mthread_create(&th1, &tab_sum, (void*)(&i1));
  mthread_create(&th2, &tab_sum, (void*)(&i2));
  mthread_join(th1, NULL); 
  mthread_join(th2, NULL);

  if (affiche_arg)
    printf("%u:%u %lu\t",a,b, tableau[a] + tableau[m+1]);
  tableau[a] += tableau[m+1];  

  return NULL;
}

