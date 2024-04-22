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
   rg u16 remainder = amt % readrate;
   rg u8 firsttime = 1;
   

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
   { // first time, and double reposition is off...fix this)
      aer0(read(in, data, readrate) != read_failed, read);
      if (gem)
      {
         if (firsttime)
         {
            decryptstr(data, remainder, gem);
            firsttime = 0;
         }
         else
         {
            decryptstr(data, readrate, gem);
         }
      }
      aer0(write(out, data, readrate) != write_failed, write);
      if (times < 2) // meets 2 conditions: when only remainder exists OR last time
      {
         break;
      }
      aer0(lseek(in, -(readrate << 1), SEEK_CUR) != lseek_failed, lseek);
      aer0(lseek(out, -(readrate << 1), SEEK_CUR) != lseek_failed, lseek);
      --times;
   } while (times);
   
   freestr(&s);
   return 1;
}


#endif // RW_H
