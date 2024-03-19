#ifndef RW_H
#define RW_H

// our libs
#include "../one/water.h"
#include "../one/units.h"

// their libs
#include <unistd.h>

// types
typedef i64 STREAM;

// defs
#define fast_aligned_rate (0b01100001100111000010000000000000) // 2568mbs per read
#define stupidfast_aligned_rate (0b01111111111111111111111111111111)

#define ALIGNED_RATE fast_aligned_rate // aligned rate (don't change)

#define lseek_failed -1
#define read_failed -1
#define write_failed -1

#define FILEMODE 1
#define SOCKETMODE 0
u8 writemode = FILEMODE;

stinl u8 writeit(STREAM out, void* data)
{
   asserterrnoret0(write(out, data, ALIGNED_RATE) != write_failed, write);
   return 1;
}

stinl u8 readit(STREAM in, u64 amt, STREAM out)
{
   // either smaller or equal to ALIGNED_RATE
   str* s = hbuffer(ALIGNED_RATE); 
   u8* data = s->array;
   rg u64 times = amt / ALIGNED_RATE; 
   rg u16 remainder = amt % ALIGNED_RATE;
   
   while (times)
   {
      asserterrnoret0(read(in, data, ALIGNED_RATE) != read_failed, read);
      assertret0(writeit(out, data) != 0, a write failed in readit);
      --times;
   }
   
   if (remainder)
   {
         asserterrnoret0(read(in, data, ALIGNED_RATE) != read_failed, read);
         assertret0(writeit(out, data) != 0, a write failed in readit);
         ftruncate(out, amt); // correct the new file size
   }
   return 1;
}

#endif // RW_H
