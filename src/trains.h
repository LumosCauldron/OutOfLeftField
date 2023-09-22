#ifndef TRAIN_H
#define TRAIN_H

#include "base/water.h"

typedef void (*freemem)(void*);
struct _train
{
   struct _train* prev;
   struct _train* next;
   void* cargo;
   freemem freecargo;
};
typedef struct _train train;


stinl train* train_new(void* cargo, freemem freecargo)
{
   train* freight = give(sizeof(train));
   freight->prev = nullptr;
   freight->next = nullptr;
   freight->cargo = cargo;
   freight->freecargo = freecargo;
   return freight;
}

stinl void train_chain(train* tank, train* gondola)
{
   if (tank)
      tank->next = gondola;
   if (gondola)
      gondola->prev = tank;
}

stinl void train_double_chain(train* tank1, train* tank2)
{
   // create cycle
   train_chain(tank1, tank2);
   train_chain(tank2, tank1);
}

stinl void train_unchain(train* freight)
{
   assertion(freight, nnn freight given to unchain);
   
   // patch the chain
   if (freight->prev)
      freight->prev->next = freight->next;
   if (freight->next)
      freight->next->prev = freight->prev;
   
   freight->prev = nullptr;
   freight->next = nullptr;
}

stinl train* train_push(train* top, train* newtop)
{
   assertret(top, nnn head given to push);
   
   if (!newtop)
      return top;
      
   if (!top->next)
      train_double_chain(newtop, top);
   else
   {
      train_chain(top->prev, newtop);
      train_chain(newtop, top);
   }  
   return newtop;
}

stinl train* train_pop(train* top)
{
   if (!top)
      return nullptr;

   train* newtop = top->next;
   train_unchain(top);
   
   // If popping last on
   if (newtop == top)
      return nullptr;
   return newtop;
}

stinl void train_free(void* ptr)
{
   if (!ptr)
      return;
   if (!invoidptr(ptr))
      return;
   
   train* freight = cast(invoidptr(ptr), train*);
   
   // patch the chain
   train_unchain(freight);
   
   // frees cargo
   // zeros out cargo and custom free function
   if (freight->freecargo)
   {
      (freight->freecargo)(&(freight->cargo));
      freight->freecargo = nullptr;
   }
   
   restore(freight);
   invoidptr(ptr) = nullptr;
}

stinl void train_free_all(void* ptr)
{
   if (!ptr)
      return;
   if (!invoidptr(ptr))
      return;
   
   train* oldtop = cast(invoidptr(ptr), train*);
   train* newtop;
   while (oldtop)
   {
      newtop = train_pop(oldtop);
      train_free(&oldtop);
      oldtop = newtop;
   }
   
   invoidptr(ptr) = nullptr;
}

stinl u64 train_len(train* startcar, u8 direction)
{
   // save where we started
   if (!startcar)
      return 0;
      
   train* original = startcar;
   u64 count = 0;
   
   if (direction == prevd)
   {
      do
      {
         ++count;
         startcar = startcar->prev;
      } while (startcar && startcar != original);
   }
   else // direction is nextd
   {
      do
      {
         ++count;
         startcar = startcar->next;
      } while (startcar && startcar != original);
   }
   return count;
}

stinl void train_reaction(train* startcar, 
                   void (*train_module)(train* t, void* misc), 
                   void* misc, 
                   u8 direction)
{
   // save where we started
   if (!startcar)
      return;
      
   train* original = startcar;
   
   if (direction == prevd)
   {
      do
      {
         if (train_module)
            (*train_module)(startcar, misc);
         startcar = startcar->prev;
      } while (startcar && startcar != original);
   }
   else // direction is nextd
   {
      do
      {
         if (train_module)
            (*train_module)(startcar, misc);
         startcar = startcar->next;
      } while (startcar && startcar != original);
   }
}

#endif // TRAIN_H
