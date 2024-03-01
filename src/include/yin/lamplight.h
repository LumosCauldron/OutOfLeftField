#ifndef LAMPLIGHT_H
#define LAMPLIGHT_H

// our libs
#include "../one/water.h"
#include "../one/units.h"

// their libs
#include <unistd.h>

// types
typedef i64 STREAM;

// defs
#define ALIGNED_RATE 4096 // important value
#define FAST_ALIGNED_READ_RATE (0b01100001100111000010000000000000) // 2568mbs per read
#define FAST_ALIGNED_WRITE_RATE (0b01100001100111000010000000000000) // 2568mbs per write
#define BLACKBOX_SIGNALED_STOP 7
#define READOP 4
#define WRITEOP 2
#define ISALIGNED (ALIGNED_RATE - 1)
#define LSEEK_FAILED -1
#define READ_FAILED -1
#define WRITE_FAILED -1

// functions
// _______________________________________________________
// increase_to_aligned_amount
//     increase the buffer size to an ALIGNED_RATE
// 
// _______________________________________________________
stinl u64 increase_to_aligned_amount(u64 amt)
{
   rg u64 x = ((amt) & ISALIGNED); // mod 
   
   // if not already aligned push up to an aligned amt
   if (x)
   {
      amt += (ALIGNED_RATE - x);
   }

   return amt;
}
// _______________________________________________________
// pourless
//     decrease the rate to ALIGNED_RATE so we do not 
//     need to allocate more than the function:
//         increase_to_aligned_amount(somebuffersize)
//     which runs at the beginning
// _______________________________________________________
stinl u32 pourless(u32 leftover, u32 rate)
{
   // half the rate each loop and then increase slightly to aligned amount
   while (rate > ALIGNED_RATE)
   {
      rate = increase_to_aligned_amount(rate >> 1);
      
      // rate will always be >= ALIGNED_RATE
      if (rate <= ALIGNED_RATE)
      {
         rate = ALIGNED_RATE;
         break;
      }
      
      // if rate <= than leftover, report back new rate
      if (rate <= leftover)
      {
         break;
      } // else half the rate again
   } 
   
   return rate;
}

// _______________________________________________________
// Oil
//     you will do it and do it well
//
// _______________________________________________________
stinl i64 Oil(STREAM portal, void* dest, u64 quantity, u32 quota, u8 optype, u8 direction)
{
   rg i64 amount = 0; // Keep SIGNED
   rg i64 iamount = 0;
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
         // guaranteed to never go negative file beginning
         if (!direction) // reading backwards
         {
            lseek(portal, -(quota + iamount), SEEK_CUR); 
         
            // program/debug error check
            asserterrnoretx(position > LSEEK_FAILED, 
                            \nposix lseek error, 
                            LSEEK_FAILED);
         }
         iamount = read(portal, dest + amount, quota);
         amount += iamount;
      }
      // program/debug error check
      asserterrnoretx(iamount > READ_FAILED, \nposix read error , READ_FAILED);
      
      if (amount == quota) // if just right
      {
         return quota; // terminating condition
      }
      else if (amount < quota) // if not enough read continue
      {
         if (quantity < quota) // quantity is less than quota (as in, its the new limit for amount)
         {
            if (amount == quantity)
            {
               return quantity; 
            }
            else if (amount < quantity) // continue reading
            {
               continue;
            } 
            else // amount > quantity (if too much read)
            {
               // 'goback' will be negative as amount is confirmed positive 
               //    and larger than confirmed positive 'quantity'
               
               // will never go back more than 'quantity' bytes
               rg i64 goback = cast(quantity, i64) - amount; 
               
               // guaranteed to never go negative file beginning
               rg i64 position = lseek(portal, goback, SEEK_CUR); 
               
               // program/debug error check
               asserterrnoretx(position > LSEEK_FAILED, 
                               \nposix lseek error, 
                               LSEEK_FAILED);
               return quantity;
            }
            // from here we flow down to "while(1);"
         }
         else  // continue reading (normal case)
         {
            continue;
         }
      }  
      else // amount > quota (if too much read)
      {  
         // 'goback' will be negative as amount is confirmed positive 
         //    and larger than confirmed positive 'quota'
         
         // will never go back more than 'quota' bytes
         rg i64 goback = cast(quota, i64) - amount; 
         
         // guaranteed to never go negative file beginning
         rg i64 position = lseek(portal, goback, SEEK_CUR); 
         
         // program/debug error check
         asserterrnoretx(position > LSEEK_FAILED && (position & ISALIGNED == 0), 
                         \nfailed to realign -- posix lseek error , 
                         LSEEK_FAILED);
                         
         // we over did it by 'goback' bytes, so we moved back the file descriptor position. 
         // it is as if only 'quota' bytes were read
         // the next read will start at the correct position in the stream/array
         return quota;
      }
   } while (1);
   
   // dead code
   return READ_FAILED;
}


// _______________________________________________________
// pacman (single thread only!)
//     data->array may be updated
   str* pacman_storage = nullptr;
   u8 pacman_storage_active = 0;
// _______________________________________________________


stinl u64 pacman(str* data, blackbox nom, u8 direction) // nom should returns leftover (e.g. a u16 leftover makes 2 bytes leftover)
{
   rg u64 leftover;
   
   // check if leftover bytes exist (for backwards case only)
   if (pacman_storage)
   {
      
      if (pacman_storage_active && !direction) // !direction is for robustness
      {  // set by setlen in this function below
         rg u64 pacman_storage_len = getlen(pacman_storage); 
         
         // doesn't check bounds, assuming readfile treats us well with the nom_max_leftover variable
         memto(data->array + getlen(data), pacman_storage->array, pacman_storage_len);
         incrlen(data, pacman_storage_len);
         pacman_storage_active = 0; 
      }
   }
   
   if (leftover = nom(data)) // <- all processing is in call to nom(data), below is just offset calculation *rolls eyes*
   {
      rg u64 where_left_off = getlen(data) - leftover;
      
      if (direction) // forward, put leftover bytes at beginning
      {
         memto(data->array, cast(data->array, u8*) + where_left_off, leftover);
      }
      else // backwards, save for next call to pacman
      {
         memto(pacman_storage->array, data->array, leftover);
         setlen(pacman_storage, leftover);
         pacman_storage_active = 1;
      }
   }
   return leftover;
}

// _______________________________________________________
// readfile
//     'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 readfile(STREAM portal, void* destptr, u64 quantity, blackbox nom, u32 nom_max_leftover, u32 quota, u8 datatype, u8 direction)
{
   sayline(~~~~~~~~~~~~~~~~~~);
   assertret0(portal >= 0, bad portal in readfile);
   assertret0(dest, nnn dest in readfile);
   
   // set dest (can be in register because its never dereferenced)
   rg u8* dest = cast(destptr, u8*);
   
   // set pacman static state array
   // only if we're reading backwards
   if (!direction) 
   {
      pacman_storage = charspace(nom_max_leftover);
   }
   
   // do not change the signs
   rg i64 amount = 0x7fffffffffffffff & quantity;
   rg i64 amt; // no need to update 'amt', will happen before next read
   rg str* temp;
   rg u64 leftover = 0;
   
   // successful read of zero bytes
   if (!amount)
   {
      return 1;
   }
   
   // fail to read from bad descriptor, read to nullptr dest, read a negative amount
   if (portal < 0 || !dest)
   {
      return 0;
   }
   
   // read and process each quota of bytes
   do
   {
      // will decrease quota down to at least ALIGNED_RATE
      if (amount < quota)
      {  
         quota = pourless(amount, quota);
      }
   
      //say(<);
      
      // !!! ACTUAL READ FUNCTION !!!
      // ensures that quota is met OR amount smaller than quota is read
      amt = 0; // update 'amt' for next write
      amt += Oil(portal, dest, cast(amount, u64), quota - leftover, READOP, direction);
      assertret0(amt > READ_FAILED, \nposix read error );
      assertret0(amt == quota || (amt == amount && amount < quota), \nwe are not able to read any more bytes);
      
      // process what we read if needed
      // note that charstr, shortstr, longstr, and intstr are stack declarations
      //    their usage must be within SCOPE of the switch statement brackets
      if (nom)
      {
         switch (datatype)
         {
            case t1: ; temp = charstr(amt, dest);
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       if (direction) // if nextd
                       {
                         dest += (leftover = pacman(temp, nom, direction));
                       } break;        
            case t2: ; temp = shortstr(amt, dest);
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       if (direction)
                       {
                         dest += (leftover = pacman(temp, nom, direction));
                       } break;   
            case t8: ; temp = longstr(amt, dest);
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       if (direction)
                       {
                         dest += (leftover = pacman(temp, nom, direction));
                       } break;   
            case t4: ; temp = intstr(amt, dest); 
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       if (direction)
                       {
                         dest += (leftover = pacman(temp, nom, direction));
                       } break;   
            default: ; temp = charstr(amt, dest); 
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       if (direction)
                       {
                         dest += (leftover = pacman(temp, nom, direction));
                       } break;   
         }
      }
      else
      {  // only increment 'dest' if we haven't dealt with the read in data yet
         dest = cast((cast(dest, u8*) + amt), void*);
      }
      
      // update amount left to read
      amount -= amt;
      
   } while (amount > 0);
   
   sayline(~~~~~~~~~~~~~~~~~~);
   return 1;
}
// _______________________________________________________
// writefile
//     'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 writefile(STREAM portal, void* dest, u64 quantity, blackbox nom, u32 nom_max_leftover, u32 quota, u8 datatype, u8 direction)
{
   sayline(^^^^^^^^^^^^^^^^^^);
   assertret0(portal >= 0, bad portal in writefile);
   assertret0(dest, nnn dest in writefile);
   
   // set dest (can be in register because its never dereferenced)
   rg u8* dest = cast(destptr, u8*);
   
   // do not change the signs
   rg i64 amount = 0x7fffffffffffffff & quantity;
   rg i64 amt = quota; // update 'amt' for first use by "nom" code
   rg str* temp;
   rg u64 leftover = 0;
   
   // successful write of zero bytes
   if (!amount)
   {
      return 1;
   }
   
   // fail to write to bad descriptor, write from nullptr dest, write a negative amount
   if (portal < 0 || !dest)
   {
      return 0;
   }
   
   // process and write each quota of bytes
   do
   {
      // process before writing
      // note that charstr, shortstr, longstr, and intstr are stack declarations
      //      their usage must be within SCOPE of the switch statement brackets
      if (nom)
      {
         switch (datatype)
         {
            case t1: ; temp = charstr(amt, dest);
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       dest += (leftover = pacman(temp, nom, direction));
                       break;        
            case t2: ; temp = shortstr(amt, dest);
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       dest += (leftover = pacman(temp, nom, direction));
                       break;   
            case t8: ; temp = longstr(amt, dest);
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       dest += (leftover = pacman(temp, nom, direction));
                       break;   
            case t4: ; temp = intstr(amt, dest); 
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       dest += (leftover = pacman(temp, nom, direction));
                       break;   
            default: ; temp = charstr(amt, dest); 
                       dest = cast(destptr, u8*); // reset here (don't move this)
                       dest += (leftover = pacman(temp, nom, direction));
                       break;   
         }
      }
      
      // will decrease quota down to at least ALIGNED_RATE
      if (amount < quota)
      {  
         quota = pourless(amount, quota);
      }
      
      // say(>);
      
      // !!! ACTUAL WRITE FUNCTION !!!
      // ensures that quota is met OR amount smaller than quota is written
      amt = 0; // update 'amt' for next write
      amt += Oil(portal, dest, cast(amount, u64), quota - leftover, WRITEOP, nextd);
      assertret0(amt > WRITE_FAILED, \nposix write error );
      assertret0(amt == quota || (amt == amount && amount < quota), \nwe are not able to write any more bytes);
      
      // if no processing needed
      if (!nom)
      {  // only increment 'dest' if we haven't dealt with the write out data yet
         dest = cast((cast(dest, u8*) + amt), void*);
      }
      
      // decrease amount we still need to write
      amount -= amt;
   } while (amount > 0);
   
   sayline(^^^^^^^^^^^^^^^^^^);
   return 1;
} 

#endif // LAMPLIGHT_H
