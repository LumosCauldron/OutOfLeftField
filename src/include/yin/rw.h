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
   str* s = hbuffer(rwFAR); 
   rg u8* data = s->array;
   rg u64 times = amt / rwFAR; 
   rg u16 remainder = amt % rwFAR;
   
   while (times)
   {
      aer0(read(in, data, rwFAR) != read_failed, read);
      aer0(write(out, data, rwFAR) != write_failed, write);
      --times;
   }
   
   if (remainder)
   {
         aer0(read(in, data, rwFAR) != read_failed, read);
         aer0(write(out, data, rwFAR) != write_failed, write);
   }
   
   freestr(&s);
   return 1;
}

stinl u8 read_backward(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   str* s = hbuffer(rwFAR); 
   rg u8* data = s->array;
   rg u16 remainder = amt % rwFAR;
   rg u64 times = (amt / rwFAR); 

   // calibrate first read (its ok if remainder is 0)
   if (remainder)
   {
      aer0(lseek(in, -remainder, SEEK_END) != lseek_failed, lseek);
      aer0(lseek(out, -remainder, SEEK_END) != lseek_failed, lseek);
   }
   else
   {
      aer0(lseek(in, -rwFAR, SEEK_END) != lseek_failed, lseek);
      aer0(lseek(out, -rwFAR, SEEK_END) != lseek_failed, lseek);
   }
         
   do
   { // first time, and double reposition is off...fix this)
      aer0(read(in, data, rwFAR) != read_failed, read);
      aer0(write(out, data, rwFAR) != write_failed, write);
      if (times < 2) // meets 2 conditions: when only remainder exists OR last time
      {
         break;
      }
      aer0(lseek(in, -(rwFAR << 1), SEEK_CUR) != lseek_failed, lseek);
      aer0(lseek(out, -(rwFAR << 1), SEEK_CUR) != lseek_failed, lseek);
      --times;
   } while (times);
   
   freestr(&s);
   return 1;
}


#endif // RW_H
