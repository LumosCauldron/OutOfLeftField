#ifndef RW_H
#define RW_H

// our libs
#include "../one/water.h"
#include "../one/units.h"

// their libs
#include <unistd.h>

// types
typedef i64 STREAM;

typedef struct
{
   void* toolbox;
   str* data;
   u8 conditions;
} nomtools;

typedef struct
{
   blackbox nom;
   u64 nom_space;
} nompkg;

typedef struct 
{
   str* data;
   u8 state;
   u8 mode;
} pacmem;

// defs
#define aligned_rate 4096 // important value
#define fast_aligned_read_rate (0b01100001100111000010000000000000) // 2568mbs per read
#define fast_aligned_write_rate (0b01100001100111000010000000000000) // 2568mbs per write
#define isaligned (aligned_rate - 1)
#define lseek_failed -1
#define read_failed -1
#define write_failed -1

// do not change these
#define readop 0b00000001
#define writeop 0b00000000
#define nextop 0b00000010
#define prevop 0b00000000
#define isread(x) (0b00000001 & (u8)(x))
#define isnext(x) (0b00000010 & (u8)(x))
#define direction_and_type(dir, type) ((dir << 1) | type)
#define pacman_read_forward (nextop | readop)
#define pacman_read_backward (prevop | readop)
#define pacman_active 1
#define pacman_not_active 0

// functions
// _______________________________________________________
// increase_to_aligned_amount
//     increase the buffer size to aligned_rate
// 
// _______________________________________________________
stinl u64 increase_to_aligned_amount(u64 amt)
{
   rg u64 x = ((amt) & isaligned); // mod 
   
   // if not already aligned push up to an aligned amt
   if (x)
   {
      amt += (aligned_rate - x);
   }

   return amt;
}
// _______________________________________________________
// pourless
//     decrease the rate to aligned_rate so we do not 
//     need to allocate more than the function:
//         increase_to_aligned_amount(somebuffersize)
//     which runs at the beginning
// _______________________________________________________
stinl u32 pourless(u32 amountleft, u32 rate)
{
   // half the rate each loop and then increase slightly to aligned amount
   while (rate > aligned_rate)
   {
      rate = increase_to_aligned_amount(rate >> 1);
      
      // rate will always be >= aligned_rate
      if (rate <= aligned_rate)
      {
         rate = aligned_rate;
         break;
      }
      
      // if rate <= than amountleft, report back new rate
      if (rate <= amountleft)
      {
         break;
      } // else half the rate again
   } 
   
   return rate;
}

// _______________________________________________________
// Oil
//     'dt' holds direction in bit 0b00000010 
//     and read/write in bit 0b00000001
// _______________________________________________________
stinl i64 Oil(STREAM portal, void* dest, u64 quantity, u32 quota, u8 dt)
{
   // do not change signs
   rg i64 amount = 0;
   rg i64 iamount = 0;
   do
   {
      explain(%s%lu\n, "reading at rate of ", quota);
      if (!isread(dt)) // writing
      {
         iamount = write(portal, dest + amount, quota);
         amount += iamount;
      }
      else // reading
      {
         // guaranteed to never go negative file beginning
         if (!isnext(dt)) // reading backwards
         {
            lseek(portal, -(quota + iamount), SEEK_CUR); 
         
            // program/debug error check
            asserterrnoretx(position > lseek_failed, 
                            \nposix lseek error, 
                            lseek_failed);
         }
         iamount = read(portal, dest + amount, quota);
         amount += iamount;
      }
      // program/debug error check
      asserterrnoretx(iamount > read_failed, \nposix read error , read_failed);
      
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
               asserterrnoretx(position > lseek_failed, 
                               \nposix lseek error, 
                               lseek_failed);
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
         asserterrnoretx(position > lseek_failed && (position & isaligned == 0), 
                         \nfailed to realign -- posix lseek error , 
                         lseek_failed);
                         
         // we over did it by 'goback' bytes, so we moved back the file descriptor position. 
         // it is as if only 'quota' bytes were read
         // the next read will start at the correct position in the stream/array
         return quota;
      }
   } while (1);
   
   // dead code
   return read_failed;
}


// _______________________________________________________
// consume (single thread only!)
//       assumes data, pkg, pacman are all valid
// _______________________________________________________

stinl u64 consume(str* data, nompkg* pkg, void* toolbox, pacmem* pacman, u8 is_it_the_last_bite)
{
   rg u64 leftovers;
   
   if (pacman->state) // is on
   {
      if (pacman->mode == pacman_read_backward)
      {
         // retrieve leftovers in pacman and append (the end will processed first)
         rg u64 pacman_data_len = getlen(pacman->data);
         memto(data->array + getlen(data), pacman->data->array, pacman_data_len);
         incrlen(data, pacman_data_len);
         pacman->state = 0; // pacman state to off
      }
   }
   
   nomtools food = { toolbox, data, is_it_the_last_bite };
   leftovers = nom(&food);
   
   if (!leftovers)
   {
      return 0;
   }
   else
   {
      switch (pacman->mode)
      {
         case pacman_read_forward : ; // put leftovers at beginning and return dest offset in read/writefile functions
                                    rg u64 where_left_off = getlen(data) - leftovers;
                                    memto(data->array, data->array + where_left_off, leftovers);
                                    return leftovers;
              break;
         case pacman_read_backward : ; // save leftovers at the beginning of data in pacman data
                                     memto(pacman->data->array, data->array, leftovers);  // pacman data array
                                     setlen(pacman->data, leftovers); // pacman data len
                                     pacman->state = 1; // pacman state to on
                                     return 0;
              break;
         default : ; // do nothing
                   return 0;
      }
   }
}

// _______________________________________________________
// readfile
//     'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 readfile(STREAM portal, void* destptr, u64 quantity, nompkg* pkg, void* toolbox, u32 quota, u8 dirntype)
{
   sayline(~~~~~~~~~~~~~~~~~~);
   assertret0(portal >= 0, bad portal in readfile);
   assertret0(dest, nnn dest in readfile);
   assertret0(quantity, asking to read zero bytes in readfile);
   
   // set dest (can be in register because its never dereferenced)
   rg u8* dest = cast(destptr, u8*);
   
   // do not change the signs
   rg i64 amount = 0x7fffffffffffffff & quantity;
   rg i64 amt; // no need to update 'amt', will happen before next read
   rg u64 leftover = 0;
   
   // successful read of zero bytes
   if (!amount)
   {
      return 1;
   }
   
   // fail to read from bad descriptor, read to nullptr dest
   if (portal < 0 || !dest)
   {
      return 0;
   }
   
   // if quota not set
   if (!quota)
   {
      quota = amount / 10; // default behavior
      if (!quota)
      {
         quota = 1;
      }
   }
   
   // if no processing package, then just make pacman small ignore it
   rg u64 pacspace = 1;
   if (pkg) 
   {
      pacspace = pkg->nom_space;
   }
   
   pacmem pacman = { charspace(pacspace), pacman_not_active, dirntype }; // start here
   
   // read and process each quota of bytes
   do
   {
      // will decrease quota down to (or upto) at least aligned_rate
      if (amount < quota)
      {  
         quota = pourless(amount, quota);
      }
   
      //say(<);
      
      // !!! ACTUAL READ FUNCTION !!!
      // ensures that quota is met OR amount smaller than quota is read
      amt = 0; // update 'amt' for next write
      amt += Oil(portal, dest, cast(amount, u64), quota - leftover, dirntype);
      assertret0(amt > read_failed, \nposix read error );
      assertret0(amt == quota || (amt == amount && amount < quota), \nwe are not able to read any more bytes);
      
      // process what we read if needed
      // note that charstr, shortstr, longstr, and intstr are stack declarations
      //    their usage must be within SCOPE of the switch statement brackets
      if (pkg)
      {
         // update amount left to read (do it before to pass end condition to nom through pacman)
         amount -= amt;
         dest = cast(destptr, u8*); // reset here (don't move this)
         rg str* temp = charstr(dest, amt + leftover);
         if (isnext(dirntype))
         { // push dest up so it does not overwrite leftovers set to beginning by consume function
           dest += (leftover = consume(temp, pkg, toolbox, pacman, (amount > 0)));
         }
      }
      else
      {  // only increment 'dest' if we haven't dealt with the read in data yet
         dest += amt;
         // update amount left to read
         amount -= amt;
      }
   } while (amount > 0);
   
   sayline(~~~~~~~~~~~~~~~~~~);
   return 1;
}
// _______________________________________________________
// writefile
//     'amount' MUST be SIGNED and 64 bits long
//
// _______________________________________________________
stinl u8 writefile(STREAM portal, void* dest, u64 quantity, u32 quota)
{
   sayline(^^^^^^^^^^^^^^^^^^);
   assertret0(portal >= 0, bad portal in writefile);
   assertret0(dest, nnn dest in writefile);
   assertret0(quantity, asking to write zero bytes in writefile);
   
   // do not change the signs
   rg i64 amount = 0x7fffffffffffffff & quantity;
   rg i64 amt = quota; // update 'amt' for first use by "nom" code
   
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
   
   // if quota not set
   if (!quota)
   {
      quota = amount / 10; // default behavior
      if (!quota)
      {
         quota = 1;
      }
   }
     
   // process and write each quota of bytes
   do
   {
      // will decrease quota down to at least aligned_rate
      if (amount < quota)
      {  
         quota = pourless(amount, quota);
      }
      
      // say(>); 
      
      // !!! ACTUAL WRITE FUNCTION !!!
      // ensures that quota is met OR amount smaller than quota is written
      amt = 0; // update 'amt' for next write
      amt += Oil(portal, dest, cast(amount, u64), quota, direction_and_type(nextop, writeop));
      assertret0(amt > write_failed, \nposix write error );
      assertret0(amt == quota || (amt == amount && amount < quota), \nwe are not able to write any more bytes);
      
      dest = cast(cast(dest, u8*) + amt, void*);
      
      // decrease amount we still need to write
      amount -= amt;
   } while (amount > 0);
   
   sayline(^^^^^^^^^^^^^^^^^^);
   return 1;
} 

#endif // RW_H
