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
#define ALIGNED_RATE 4096 // =-> important value
#define FAST_ALIGNED_RATE (0b00000100000100010011000000000000) // =-> I am speed
#define BLACKBOX_SIGNALED_STOP 7
#define READOP 4
#define WRITEOP 2
#define INCREASE_TO_ALIGNED_AMOUNT(amt) ((amt) + (ALIGNED_RATE - ((amt) & ISALIGNED)))
#define ISALIGNED (ALIGNED_RATE - 1)
#define LSEEK_FAILED -1
#define READ_FAILED -1
#define WRITE_FAILED -1

// functions
// _______________________________________________________
// pourless
//      decrease the rate to ALIGNED_RATE so we do not 
//      need to allocate more than the macro:
//          INCREASE_TO_ALIGNED_AMOUNT(somebuffersize)
// _______________________________________________________
stinl u32 pourless(u32 leftover, u32 rate)
{
   // =-> half the rate each loop and then increase slightly to aligned amount
   while (rate > ALIGNED_RATE)
   {
      rate = INCREASE_TO_ALIGNED_AMOUNT(rate >> 1);
      
      // =-> rate will always be >= ALIGNED_RATE
      if (rate <= ALIGNED_RATE)
      {
         rate = ALIGNED_RATE;
         break;
      }
      
      // =-> if rate <= than leftover, report back new rate
      if (rate <= leftover)
      {
         break;
      } // =-> else half the rate again
   } 
   
   return rate;
}
// _______________________________________________________
// Oil
//      you will do it and do it well
//
// _______________________________________________________
stinl i64 Oil(STREAM portal, void* dest, u64 quantity, u32 quota, u8 optype)
{
   register i64 amount = 0; // =-> Keep SIGNED
   register i64 iamount = 0;
   do
   {
      explain(%s%lu\n, "reading at rate of ", quota);
      if (optype == WRITEOP)
      {
         iamount = write(portal, dest + amount, quota);
         amount += iamount;
      }
      else // optype == READOP
      {
         iamount = read(portal, dest + amount, quota);
         amount += iamount;
      }
      // =-> program/debug error check
      asserterrnoretx(iamount > READ_FAILED, \nposix read error , READ_FAILED);
      
      if (amount == quota) // =-> if just right
      {
         return quota; // =-> terminating condition
      }
      else if (amount < quota) // =-> if not enough read continue
      {
         if (quantity < quota) // =-> quantity is less than quota (as in, its the new limit for amount)
         {
            if (amount == quantity)
            {
               return quantity; 
            }
            else if (amount < quantity) // =-> continue reading
            {
               continue;
            } 
            else // =-> amount > quantity (if too much read)
            {
               // =-> 'goback' will be negative as amount is confirmed positive 
               //     and larger than confirmed positive 'quantity'
               
               // =-> will never go back more than 'quantity' bytes
               register i64 goback = cast(quantity, i64) - amount; 
               
               // =-> guaranteed to never go negative file beginning
               register i64 position = lseek(portal, goback, SEEK_CUR); 
               
               // =-> program/debug error check
               asserterrnoretx(position > LSEEK_FAILED, 
                               \nposix lseek error, 
                               LSEEK_FAILED);
               return quantity;
            }
            // =-> from here we flow down to "while(1);"
         }
         else  // =-> continue reading (normal case)
         {
            continue;
         }
      }  
      else // =-> amount > quota (if too much read)
      {  
         // =-> 'goback' will be negative as amount is confirmed positive 
         //     and larger than confirmed positive 'quota'
         
         // =-> will never go back more than 'quota' bytes
         register i64 goback = cast(quota, i64) - amount; 
         
         // =-> guaranteed to never go negative file beginning
         register i64 position = lseek(portal, goback, SEEK_CUR); 
         
         // =-> program/debug error check
         asserterrnoretx(position > LSEEK_FAILED && (position & ISALIGNED == 0), 
                         \nfailed to realign -- posix lseek error , 
                         LSEEK_FAILED);
                         
         // =-> we over did it by 'goback' bytes, so we moved back the file descriptor position. 
         // =-> it is as if only 'quota' bytes were read
         // =-> the next read will start at the correct position in the stream/array
         return quota;
      }
   } while (1);
   
   // =-> dead code
   return READ_FAILED;
}

// _______________________________________________________
// Lamp
//      'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 Lamp(STREAM portal, void* dest, u64 quantity, BLACKBOX dealwith, u32 quota, u8 datatype)
{
   sayline(~~~~~~~~~~~~~~~~~~);
   assertret0(portal >= 0, bad portal in Lamp);
   assertret0(dest, nnn dest in Lamp);
   
   // =-> do not change the signs
   register i64 amount = 0x7fffffffffffffff & quantity;
   register i64 amt; // =-> no need to update 'amt', will happen before next read
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
   
   // =-> read and process each quota of bytes
   do
   {
      // =-> will decrease quota down to at least ALIGNED_RATE
      if (amount < quota)
      {  
         quota = pourless(amount, quota);
      }
   
      //say(<);
      
      // !!! ACTUAL READ FUNCTION !!!
      // =-> ensures that quota is met OR amount smaller than quota is read
      amt = 0; // =-> update 'amt' for next write
      amt += Oil(portal, dest, cast(amount, u64), quota, READOP);
      assertret0(amt > READ_FAILED, \nposix read error );
      assertret0(amt == quota || (amt == amount && amount < quota), \nwe are not able to read any more bytes);
      
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
      
      // =-> update amount left to read
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
stinl u8 Light(STREAM portal, void* dest, u64 quantity, BLACKBOX dealwith, u32 quota, u8 datatype)
{
   sayline(^^^^^^^^^^^^^^^^^^);
   assertret0(portal >= 0, bad portal in Lamp);
   assertret0(dest, nnn dest in Lamp);
   
   // =-> do not change the signs
   register i64 amount = 0x7fffffffffffffff & quantity;
   register i64 amt = quota; // =-> update 'amt' for first use by "dealwith" code
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
   
   // =-> process and write each quota of bytes
   do
   {
      // =-> process before writing
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
   
      
      
      // =-> will decrease quota down to at least ALIGNED_RATE
      if (amount < quota)
      {  
         quota = pourless(amount, quota);
      }
      
      // say(>);
      
      // !!! ACTUAL WRITE FUNCTION !!!
      // =-> ensures that quota is met OR amount smaller than quota is written
      amt = 0; // =-> update 'amt' for next write
      amt += Oil(portal, dest, cast(amount, u64), quota, WRITEOP);
      assertret0(amt > WRITE_FAILED, \nposix write error );
      assertret0(amt == quota || (amt == amount && amount < quota), \nwe are not able to write any more bytes);
      
      // =-> if no processing needed
      if (!dealwith)
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
