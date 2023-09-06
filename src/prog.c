#include "units.h"


int main()
{
   u64 ii = 0x6161646263656661;
   str* i = longstr(1, &ii);
   printstr(i);
   str* s = appendstr(i, datastr("@$#V%Y@%Q#$%BA"));
   printstr(s);
   freestr(&s);
   RETURN;
}

/* */
