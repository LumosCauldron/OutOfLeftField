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


stinl void* process_data(u8* data, u64 len, blackbox fptr, void* tools)
{
   str* s = charstr(len, data);
   u8 sixteen[sizeof(u64) << 1];
   *(cast(sixteen, u64*)) = s;
   *(cast(sixteen, u64*) + 1) = tools;
   (*fptr)(&sixteen);
}

stinl u8 readfilef(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   rg u64 readrate = (amt / rwDBS) * rwDBS;
   if (!readrate)
   {
      readrate = rwDBS;
   }
   else if (readrate > fast_aligned_rate)
   {
      readrate = fast_aligned_rate;
   }
   
   // configure number of reads/writes
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

stinl u8 readfileb(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   rg u64 readrate = (amt / rwDBS) * rwDBS;
   if (!readrate)
   {
      readrate = rwDBS;
   }
   else if (readrate > fast_aligned_rate)
   {
      readrate = fast_aligned_rate;
   }
   
   // configure number of reads/writes
   str* s = hbuffer(readrate);
   u8* data = getarray(s);
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

#define READ_SIZE 4096
#define PROCESS_SIZE 42

i64 read_and_process(STREAM in, u64 filesz, STREAM out, CryptoGemStone* gem) 
{
    u8 buffer[READ_SIZE];
    u8 leftover[PROCESS_SIZE];
    rg u64 leftover_size = 0;

    while (filesz > 0) 
    {
        rg i64 read_size = read(in, buffer, READ_SIZE < filesz ? READ_SIZE : filesz);
        if (read_size <= 0) 
        {
            // Handle read error or EOF here
            break;
        }

        filesz -= read_size;

        // If there are leftover bytes from the last read, prepend them to the buffer
        if (leftover_size > 0) {
            memto(buffer + leftover_size, buffer, read_size);
            memto(buffer, leftover, leftover_size);
            read_size += leftover_size;
            leftover_size = 0;
        }

        // Process data in chunks of PROCESS_SIZE
        rg u64 i;
        for (i = 0; i + PROCESS_SIZE <= read_size; i += PROCESS_SIZE) 
        {
            process_data(buffer + i, PROCESS_SIZE);
            // Write processed data to out
            write(out, buffer + i, PROCESS_SIZE);
        }

        // If there are leftover bytes, save them for the next read
        if (i < read_size) 
        {
            leftover_size = read_size - i;
            memto(leftover, buffer + i, leftover_size);
        }
    }

    // Process any remaining bytes
    if (leftover_size > 0) 
    {
        process_data(leftover, leftover_size);
        // Write processed data to out
        write(out, leftover, leftover_size);
    }

    return filesz;
}

i64 read_and_process_backward(int in, int out, size_t amount) 
{
    u8 buffer[READ_SIZE];
    u8 leftover[PROCESS_SIZE];
    rg u64 leftover_size = 0;

    rg i64 file_size = lseek(in, 0, SEEK_END);
    if (file_size == -1) 
    {
        // Handle lseek error here
        return -1;
    }

    while (amount > 0) 
    {
        i64 read_offset = file_size - READ_SIZE < 0 ? 0 : file_size - READ_SIZE;
        u64 read_amount = file_size - read_offset < amount ? file_size - read_offset : amount;

        if (lseek(in, read_offset, SEEK_SET) == -1) 
        {
            // Handle lseek error here
            return -1;
        }

        i64 read_size = read(in, buffer, read_amount);
        if (read_size <= 0) 
        {
            // Handle read error or EOF here
            break;
        }

        amount -= read_size;
        file_size -= read_size;

        // If there are leftover bytes from the last read, append them to the buffer
        if (leftover_size > 0) 
        {
            memto(buffer, buffer + leftover_size, read_size);
            memto(buffer + read_size, leftover, leftover_size);
            read_size += leftover_size;
            leftover_size = 0;
        }

        // Process data in chunks of PROCESS_SIZE
        u64 i;
        for (i = 0; i + PROCESS_SIZE <= read_size; i += PROCESS_SIZE) 
        {
            process_data(buffer + i, PROCESS_SIZE);
            // Write processed data to out
            write(out, buffer + i, PROCESS_SIZE);
        }

        // If there are leftover bytes, save them for the next read
        if (i < read_size) 
        {
            leftover_size = read_size - i;
            memto(leftover, buffer + i, leftover_size);
        }
    }

    // Process any remaining bytes
    if (leftover_size > 0) {
        process_data(leftover, leftover_size);
        // Write processed data to out
        write(out, leftover, leftover_size);
    }

    return amount;
}































#define MINRATE 4096
#define PROCESS_RATE 42
#define PROCESS_SPAN 49

// 1. set rate & space
// while amt > 0
// 2. read
// 3. process
// 4. write
// 5. save unprocessed
// 6. connect to start
// 7. read...

u8 read_fw(STREAM in, i64 amt, STREAM out, blackbox b)
{
   // set remainder, times, rate
   rg u64 remainder = amt % MINRATE;
   rg u64 times = amt / MINRATE;
   rg u64 rate = times * MINRATE;
   if (!rate)
   {
      rate = MINRATE;
   }
   else if (rate > fast_aligned_rate)
   {
      rate = fast_aligned_rate;
   }
   
   // set space
   u8 space[MINRATE + rate];
   rg u8* sptr = space + MINRATE;
   
   // pre-read / handle small amt
   read(in, space, MINRATE);
   // take care of minrate/remainder/times-calc
      process(space, amt);
      write(out, space, MINRATE);
   if (amt < MINRATE)
   {
      return 1;
   }
   
   
   while (times)
   {
      // read
      read(in, sptr, rate);
      
      
      --times;
   }
   
   
   
   return 1;
}

#endif // RW_H
