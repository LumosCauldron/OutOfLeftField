#ifndef RW_H
#define RW_H

// our libs
#include "../one/water.h"
#include "../one/crypto/cipher7.h"
#include "../one/units.h"

// their libs
#include <unistd.h>

// types
typedef i64 STREAM;

// defs
#define fast_aligned_rate (max_readblock * max_readblock * 128) // ~2147mbs per read
#define encryption_aligned_rate (max_readblock * max_readblock * ENCRYPTION_INCREMENT * 3) // ~2114mbs per read
#define disk_block_size 4096
#define rwDBS disk_block_size
#define rwAR fast_aligned_rate // aligned rate (don't change)
#define rwEAR encryption_aligned_rate

#define lseek_failed -1
#define read_failed -1
#define write_failed -1

/*
CryptoGemStone gem1;
cryptokey(testKey1->array, &gem1);
encryptstr(field1->array, getlen(field1), &gem1);
decryptstr(field1->array, getlen(field1), &gem1);
*/

stinl u8 read_tunnel(STREAM in, u64 amt, STREAM out)
{
   str* s = hbuffer(rwAR); 
   rg u8* data = s->array;
   rg u64 times = amt / rwAR; 
   rg u16 remainder = amt % rwAR;
   
   while (times)
   {
      asserterrnoret0(read(in, data, rwAR) != read_failed, read);
      asserterrnoret0(write(out, data, rwAR) != write_failed, write);
      --times;
   }
   
   if (remainder)
   {
         asserterrnoret0(read(in, data, rwAR) != read_failed, read);
         asserterrnoret0(write(out, data, rwAR) != write_failed, write);
         ftruncate(out, amt); // correct the new file size !!! (maybe take this and place outside)
   }
   return 1;
}

// encryption /////////////////////////////////////////////

stinl u8 encrypt_tunnel(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   str* s = hbuffer(rwDBS + rwEAR); // +4096...still aligned
   rg u8* start = s->array;
   rg u8* tosave = start + rwEAR;
   rg u8* read_start = start + rwDBS;
   rg u8* encrypt_start = start + (rwDBS - ENCRYPTION_INCREMENT);
   rg u64 encrypt_limit = rwEAR + BLKBLKSZ;
   rg u64 times = amt / rwEAR;
   rg u16 remainder = amt % rwEAR;
   rg u16 additional_aligned_rw = 0;
   
   if (!times) // takes care of small case
   {
      goto lastread;
   }
   
   if (amt >= rwEAR && amt < (rwDBS + rwEAR)) // takes care of in between case
   {
      remainder = amt;
      read_start = start;
      encrypt_start = start;
      additional_aligned_rw = rwDBS;
      goto lastread;
   }
   
   // first read
   asserterrnoret0(read(in, start, rwDBS + rwEAR) != read_failed, read);
   encryptblocks(start, ((rwDBS + rwEAR) - ENCRYPTION_INCREMENT) + BLKBLKSZ, gem);
   asserterrnoret0(write(out, start, rwEAR) != write_failed, write);
   memto(start, tosave, rwDBS);
   
   if (!(times = (amt - (rwDBS + rwEAR)) / rwEAR))
   {
      goto lastread;
   }
   
   // middle reads
   while (times)
   {
      asserterrnoret0(read(in, read_start, rwEAR) != read_failed, read);
      encryptblocks(encrypt_start, encrypt_limit, gem);
      asserterrnoret0(write(out, start, rwEAR) != write_failed, write);
      memto(start, tosave, rwDBS);
      --times;
   }
   
   lastread:
   // last read
   if (remainder)
   {
      asserterrnoret0(read(in, read_start, rwEAR + additional_aligned_rw) != read_failed, read);
      encryptblocks(encrypt_start, remainder, gem);
      encryptend(encrypt_start, remainder, gem);
      asserterrnoret0(write(out, start, rwEAR + additional_aligned_rw) != write_failed, write);
   }
   return 1;
}

// decryption

stinl u8 decrypt_tunnel(STREAM in, u64 amt, STREAM out, CryptoGemStone* gem)
{
   str* s = hbuffer(rwDBS + rwEAR); // +4096...still aligned
   rg u8* start = s->array;
   rg u8* tosave = start + rwEAR;
   rg u8* read_start = start + rwDBS;
   rg u8* decrypt_start = start + (rwDBS - ENCRYPTION_INCREMENT);
   rg u64 decrypt_limit = rwEAR + BLKBLKSZ;
   rg u64 times = amt / rwEAR;
   rg u16 remainder = amt % rwEAR;
   rg u16 additional_aligned_rw = 0;
   
   if (!times) // takes care of small case
   {
      goto lastread;
   }
   
   if (amt >= rwEAR && amt < (rwDBS + rwEAR)) // takes care of in between case
   {
      remainder = amt;
      read_start = start;
      decrypt_start = start;
      additional_aligned_rw = rwDBS;
      goto lastread;
   }
   
   // last read
   if (remainder) // START HERE FIGURE OUT DECRYPTION (clean all this up)
   {
      asserterrnoret0(read(in, read_start, rwEAR + additional_aligned_rw) != read_failed, read);
      decryptblocks(decrypt_start, remainder, gem);
      asserterrnoret0(write(out, start, rwEAR + additional_aligned_rw) != write_failed, write);
   }
   
   // first read
   asserterrnoret0(read(in, start, rwDBS + rwEAR) != read_failed, read);
   // decrypt end based on amt which should be total data size being decrypted......
   decryptblocks(start, ((rwDBS + rwEAR) - ENCRYPTION_INCREMENT) + BLKBLKSZ, gem);
   asserterrnoret0(write(out, start, rwEAR) != write_failed, write);
   memto(start, tosave, rwDBS);
   
   if (!(times = (amt - (rwDBS + rwEAR)) / rwEAR))
   {
      goto lastread;
   }
   
   // middle reads
   while (times)
   {
      asserterrnoret0(read(in, read_start, rwEAR) != read_failed, read);
      decryptblocks(decrypt_start, decrypt_limit, gem);
      asserterrnoret0(write(out, start, rwEAR) != write_failed, write);
      memto(start, tosave, rwDBS);
      --times;
   }
   
   return 1;
}

#endif // RW_H
