#ifndef RW_H
#define RW_H

// our libs
#include "../one/water.h"
#include "../one/crypto/cipher7.h"
#include "../one/units.h"
#include "nixfiles.h"

// their libs
#include <unistd.h>

/*
CryptoGemStone gem1;
cryptokey(testKey1->array, &gem1);
encryptstr(field1->array, getlen(field1), &gem1);
decryptstr(field1->array, getlen(field1), &gem1);
*/

stinl u8 read_forward(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   rg u64 readrate = (amt / rwDBS) * rwDBS;
   if (!readrate)
   {
      readrate = rwDBS;
   }
   str* s = hbuffer(readrate); 
   rg u8* data = s->array;
   rg u64 times = amt / readrate; 
   rg u16 remainder = amt % readrate;
   
   while (times)
   {
      
      aer0(read(in, data, readrate) != read_failed, read);
      if (gem)
      {
         encryptstr(data, readrate, gem);
      }
      aer0(write(out, data, readrate) != write_failed, write);
      --times;
   }
   
   if (remainder)
   {
      aer0(read(in, data, readrate) != read_failed, read);
      if (gem)
      {
         encryptstr(data, remainder, gem);
      }
      aer0(write(out, data, readrate) != write_failed, write);
   }
   
   freestr(&s);
   return 1;
}

stinl u8 read_backward(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   rg u64 readrate = (amt / rwDBS) * rwDBS;
   if (!readrate)
   {
      readrate = rwDBS;
   }
   str* s = hbuffer(readrate); 
   rg u8* data = s->array;
   rg u64 times = (amt / readrate); 
   rg i64 backup = -(readrate << 1); // keep a signed integer
   rg u16 remainder = amt % readrate;
   

   // calibrate first read (its ok if remainder is 0)
   if (remainder)
   {
      aer0(lseek(in, -remainder, SEEK_END) != lseek_failed, lseek);
      aer0(lseek(out, -remainder, SEEK_END) != lseek_failed, lseek);
   }
   else
   {
      aer0(lseek(in, -readrate, SEEK_END) != lseek_failed, lseek);
      aer0(lseek(out, -readrate, SEEK_END) != lseek_failed, lseek);
   }
         
   do
   {
      aer0(read(in, data, readrate) != read_failed, read);
      if (gem)
      {
         if (remainder)
         {
            decryptstr(data, remainder, gem);
         }
         else
         {
            decryptstr(data, readrate, gem);
         }
      }
      aer0(write(out, data, readrate) != write_failed, write);
      // meets 2 conditions: when only remainder exists OR last time
      // this if-statement is the wall to make sure lseek doesn't place us before file
      if (times < 2) 
      {
         // we just did the remainder so go around one more time if 'times' == 1
         if (!remainder || !times)
         {
            break; // jump out of while(1)
         }
      }
      
      // go back the correct amount of bytes based off what we read
      if (remainder)
      {              // backup is a negative integer
         aer0(lseek(in, backup + cast(readrate - remainder, i64), SEEK_CUR) != lseek_failed, lseek);
      }
      else
      {
         aer0(lseek(in, backup, SEEK_CUR) != lseek_failed, lseek);
      }
      aer0(lseek(out, backup, SEEK_CUR) != lseek_failed, lseek);
      
      // "turn off" the remainder
      remainder = 0;
      --times;
   } while (1);
   
   freestr(&s);
   return 1;
}


#endif // RW_H
