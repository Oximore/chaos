#include <stdio.h>
#include <stdlib.h>
#include "listesimple.h"

typedef struct item
{
   struct item *next;
   void *data;
} item_s;

struct sll
{
   item_s *p_start;
   item_s *list;
};

sll_s *sll_new (void)
{
   sll_s *p_sll = malloc (sizeof *p_sll);

   if (p_sll)
   {
      item_s *p_l = malloc (sizeof *p_l);

      if (p_l)
      {
         p_l->data = NULL;
         p_l->next = NULL;

         p_sll->p_start = p_l;
         p_sll->list = NULL;
      }
      else
      {
         fprintf (stderr, "Memoire insufisante\n");
         exit (EXIT_FAILURE);
      }
   }
   else
   {
      fprintf (stderr, "Memoire insufisante\n");
      exit (EXIT_FAILURE);
   }
   return p_sll;
}

void sll_insert (sll_s * p_sll, void *data)
{
   if (p_sll)
   {
      item_s *p_l = p_sll->list;
      item_s *p_n = NULL;

      p_n = malloc (sizeof (*p_n));
      if (p_n)
      {
         p_n->data = data;
         if (p_l == NULL)
         {
            p_sll->p_start->next = p_n;
            p_n->next = NULL;
         }
         else
         {
            p_n->next = p_l->next;
            p_l->next = p_n;
         }
         p_sll->list = p_n;
      }
      else
      {
         fprintf (stderr, "Memoire insuffisante\n");
         exit (EXIT_FAILURE);
      }
   }
}

void sll_removeNext (sll_s * p_sll)
{
   if (p_sll && p_sll->list)
   {
      item_s *p_l = p_sll->list;
      item_s *p_n = NULL;

      p_n = p_l->next;
      p_l->next = p_n->next;
      free (p_n);
      p_n = NULL;
   }
}

void sll_removeFirst (sll_s * p_sll)
{
   if (p_sll)
   {
      p_sll->list = p_sll->p_start;
      sll_removeNext (p_sll);
   }
}

void sll_next (sll_s * p_sll)
{
   if (p_sll && p_sll->list)
   {
      p_sll->list = p_sll->list->next;
   }
}

void *sll_data (sll_s * p_sll)
{
   return ((p_sll && p_sll->list) ? p_sll->list->data : NULL);
}

void sll_first (sll_s * p_sll)
{
   if (p_sll)
   {
      p_sll->list = p_sll->p_start->next;
   }
}

void sll_last (sll_s * p_sll)
{
   if (p_sll)
   {
      while (p_sll->list->next != NULL)
      {
         sll_next (p_sll);
      }
   }
}

size_t sll_sizeof (sll_s * p_sll)
{
   size_t n = 0;

   if (p_sll)
   {
      sll_first (p_sll);
      while (p_sll->list != NULL)
      {
         n++;
         sll_next (p_sll);
      }
   }
   return n;
}

void sll_delete (sll_s ** pp_sll)
{
   if (pp_sll && *pp_sll)
   {
      sll_first (*pp_sll);

      while ((*pp_sll)->list->next != NULL)
      {
         sll_removeNext (*pp_sll);
      }
      sll_removeFirst (*pp_sll);
      free ((*pp_sll)->list);
      (*pp_sll)->list = NULL;
      free (*pp_sll), *pp_sll = NULL;
   }
}
