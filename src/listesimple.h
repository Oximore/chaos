#ifndef H_LISTESIMPLE
#define H_LISTESIMPLE

#include <stddef.h>             /* pour size_t */

typedef struct sll sll_s;

sll_s *sll_new (void);
void sll_insert (sll_s *, void *);
void sll_removeNext (sll_s *);
void sll_removeFirst (sll_s *);
void sll_next (sll_s *);
void *sll_data (sll_s *);
void sll_first (sll_s *);
void sll_last (sll_s *);
size_t sll_sizeof (sll_s *);
void sll_delete (sll_s **);

#endif /* not H_LISTESIMPLE */
