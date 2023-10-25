#ifndef LAMPLIGHT_H
#define LAMPLIGHT_H

// our libs
#include "../one/water.h"
#include "../one/units.h"

// their libs
#include <unistd.h>

// types
typedef i64 STREAM;

// function pointer types
typedef u8 (*BLACKBOX)(void*);

// defs
#define BLACKBOX_SIGNALED_STOP 7
#define ALIGNED_RATE 4096
#define ALIGNED_REMAINDER_MASK (ALIGNED_RATE - 1)
#define INCREASE_TO_ALIGNED_AMOUNT(amt) ((amt) + ((amt) - ((amt) & ALIGNED_REMAINDER_MASK)))
#define READ_FAILED -1
#define WRITE_FAILED -1

// functions
// _______________________________________________________
// Lamp
//      'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 Lamp(STREAM portal, void* dest, u64 quantity, BLACKBOX dealwith, u32 rate, u8 datatype)
{
   sayline(~~~~~~~~~~~~~~~~~~);
   assertret0(portal >= 0, bad portal in Lamp);
   assertret0(dest, nnn dest in Lamp);
   
   // =-> do not change the signs
   register i64 amount = 0x7fffffffffffffff & quantity;
   register i64 amt;
   register str* temp;
   
   // =-> successful read of zero bytes
   if (!amount)
   {
      return 1;
   }
   
   // =-> fail to read from bad descriptor, read to nullptr dest, read a negative amount
   if (portal < 0 || !dest)
   {
      return 0;
   }
   
   // =-> if rate is greater than specified amount or zero, 
   //     make it equal to the amount to read
   if (amount < rate || !rate)
   {
      rate = amount;
   }
   
   // =-> read it
   do
   {
      say(<);
      if (amount >= rate)
      {
         amt = read(portal, cast(dest, u8*), rate);
      }
      else
      {
         amt = read(portal, cast(dest, u8*), amount);
      }
      
      // =-> debug error check
      asserterrnoret0(amt > READ_FAILED, \nposix read error );
      asserterrnoret0(amt == rate || amount == amt, \ndid-not-read-up-to-rate-specified read error );
      
      // =-> program error check
      if ((amt <= READ_FAILED) || 
          (amt != rate && amt != amount))
      {
         return 0;
      }
      
      // =-> process what we read if needed
      // =-> note that charstr, shortstr, longstr, and intstr are stack declarations
      //     their usage must be within SCOPE of the switch statement brackets
      if (dealwith)
      {
         switch (datatype)
         {
            case t1: ; temp = charstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;        
            case t2: ; temp = shortstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
            case t8: ; temp = longstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
            case t4: ; temp = intstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
            default: ; temp = charstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
         }
      }
      else
      {  // =-> only increment 'dest' if we haven't dealt with the read in data yet
         dest = cast((cast(dest, u8*) + amt), void*);
      }
      
      // =-> decrease amount we still need to read
      amount -= amt;
   } while (amount > 0);
   sayline(~~~~~~~~~~~~~~~~~~);
   return 1;
}
// _______________________________________________________
// Light
//      'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 Light(STREAM portal, void* dest, u64 quantity, BLACKBOX dealwith, u32 rate, u8 datatype)
{
   sayline(^^^^^^^^^^^^^^^^^^);
   assertret0(portal >= 0, bad portal in Lamp);
   assertret0(dest, nnn dest in Lamp);
   
   // =-> do not change the signs
   register i64 amount = 0x7fffffffffffffff & quantity;
   register i64 amt;
   register str* temp;
   
   // =-> successful write of zero bytes
   if (!amount)
   {
      return 1;
   }
   
   // =-> fail to write to bad descriptor, write from nullptr dest, write a negative amount
   if (portal < 0 || !dest)
   {
      return 0;
   }
   
   // =-> if rate is greater than specified amount or zero, 
   //     make it equal to the amount to write
   if (amount < rate || !rate)
   {
      rate = amount;
   }
   
   // =-> write it
   do
   {          
      say(>);
      if (amount >= rate)
      {
         amt = write(portal, cast(dest, u8*), rate);
      }
      else
      {
         amt = write(portal, cast(dest, u8*), amount);
      }
      // =-> debug error check
      asserterrnoret0(amt > WRITE_FAILED, posix write error );
      asserterrnoret0(amt == rate || amt == amount, did-not-write-up-to-rate-specified write error );
      
      // =-> program error check
      if ((amt <= WRITE_FAILED) || 
          (amt != rate && amt != amount))
      {
         return 0;
      }
      
      // =-> process what we write if needed
      // =-> note that charstr, shortstr, longstr, and intstr are stack declarations
      //     their usage must be within SCOPE of the switch statement brackets
      if (dealwith)
      {
         switch (datatype)
         {
            case t1: ; temp = charstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;        
            case t2: ; temp = shortstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
            case t8: ; temp = longstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
            case t4: ; temp = intstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
            default: ; temp = charstr(amt, dest); 
                       if (!(dealwith(temp))) { return BLACKBOX_SIGNALED_STOP; } break;   
         } 
      }
      else
      {  // =-> only increment 'dest' if we haven't dealt with the write out data yet
         dest = cast((cast(dest, u8*) + amt), void*);
      }
      
      // =-> decrease amount we still need to write
      amount -= amt;
   } while (amount > 0);
   
   sayline(^^^^^^^^^^^^^^^^^^);
   return 1;
} 

#endif // LAMPLIGHT_H
