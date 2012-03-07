#include <stdio.h>
#include <stdlib.h>
#include "listesimple.h"

static void list_print (sll_s *);

int main (void)
{
   sll_s *p_sll = NULL;
   char text[][6] = { "Hello", " ", "World", "!" };

   printf ("\t-- Initialisation --\n");
   p_sll = sll_new ();
   list_print (p_sll);

   printf ("\n\t-- Insertion --\n");
   {
      int i;

      for (i = 0; i < 4; i++)
      {
         sll_insert (p_sll, text[i]);
      }
   }
   list_print (p_sll);

   printf ("\n\t-- Suppression --\n");
   sll_removeFirst (p_sll);
   list_print (p_sll);

   printf ("\n\t-- Destruction --\n");
   sll_delete (&p_sll);
   list_print (p_sll);

   return EXIT_SUCCESS;
}

static void list_print (sll_s * p_l)
{
   int i;
   int size = sll_sizeof (p_l);

   sll_first (p_l);
   for (i = 0; i < size; i++)
   {
      if (sll_data (p_l) != NULL)
      {
         printf ("%s", (char *) sll_data (p_l));
      }
      sll_next (p_l);
   }
   printf ("\n");
}
