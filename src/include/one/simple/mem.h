#include "types.h"
#include "def.h"

/* 
     absolute value
                      */
u64  pos(i64 num)
{ 
     if (num < 0)
     { 
          return u64c(-num);
     } 
     return u64c(num);
}

/* 
     absolute value of ptr difference
                                       */
u64 ptrdiff(void* one, void* two)
{
     return pos(u64c(one) - u64c(two));
}

/* 
     memmove in reverse; triggered by mem overlap
                                                    */
void memto_rev(void* dst, void* src, u64 len)
{
     u32 times = len >> 3;
     u8 rem = len & 7;
     
     dst = off8(dst, len);
     src = off8(src, len);
     
     for(; rem > 0; --rem)
     {
          dst = off8(dst, -1);
          src = off8(src, -1);
          in8(dst) = in8(src);
     }
     for(; times > 0; --times)
     {
          dst = off64(dst, -1);
          src = off64(src, -1);
          in64(dst) = in64(src);
     }
}

/* 
     custom memmove; handles overlap
                                       */
void memto(void* dst, void* src, u64 len)
{
     u64 diff = ptrdiff(dst, src);
     u32 times = len >> 3;
     u8 rem = len & 7;
     
     if (diff < len)
     {
          if (src < dst)
          {
               memto_rev(dst, src, len);
               return;
          }
     }
     
     for(; times > 0; --times)
     {
          in64(dst) = in64(src);
          dst = off64(dst, 1);
          src = off64(src, 1);
     }
     for(; rem > 0; --rem)
     {
          in8(dst) = in8(src);
          dst = off8(dst, 1);
          src = off8(src, 1);
     }
}

/* 
     memcmp in reverse; triggered by mem overlap
                                                   */
u8 memeq_rev(void* dst, void* src, u64 len)
{
     u32 times = len >> 3;
     u8 rem = len & 7;
     
     dst = off8(dst, len);
     src = off8(src, len);
     
     for(; rem > 0; --rem)
     {
          dst = off8(dst, -1);
          src = off8(src, -1);
          if (in8(dst) ^ in8(src))
          {
               return 0;
          }
     }
     for(; times > 0; --times)
     {
          dst = off64(dst, -1);
          src = off64(src, -1);
          if (in64(dst) ^ in64(src))
          {
               return 0;
          }
     }
     return 1;
}

/* 
     custom memcmp; handles overlap
                                       */
u8 memeq(void* dst, void* src, u64 len)
{
     u64 diff = ptrdiff(dst, src);
     u32 times = len >> 3;
     u8 rem = len & 7;
     
     if (diff < len)
     {
          if (src < dst)
          {
               return memeq_rev(dst, src, len);
          }
     }
     
     for(; times > 0; --times)
     {
          if (in64(dst) ^ in64(src))
          {
               return 0;
          }
          dst = off64(dst, 1);
          src = off64(src, 1);
     }
     for(; rem > 0; --rem)
     {
          if (in8(dst) ^ in8(src))
          {
               return 0;
          }         
          dst = off8(dst, 1);
          src = off8(src, 1);
     }
     return 1;
}

/* 
     custom memset
                     */
void mempaint(void* dst, u64 len, u8 c)
{
     u8 src[8] = { c, c, c, c, c, c, c, c };
     u32 times = len >> 3;
     u8 rem = len & 7;
     for(; times > 0; --times)
     {
          in64(dst) = in64(src);
          dst = off64(dst, 1);
     }
     for(; rem > 0; --rem)
     {
          in8(dst) = in8(src);
          dst = off8(dst, 1);
     }
}

/* 
     scrubs memory
                     */
void memscrub(void* dst, u64 len)
{
     mempaint(dst, len, 0x55);
     mempaint(dst, len, 0xAA);
}

/* 
     memxor in reverse; triggered by mem overlap
                                                   */
void memxor_rev(void* dst, void* src, u64 len)
{
     u32 times = len >> 3;
     u8 rem = len & 7;
     
     dst = off8(dst, len);
     src = off8(src, len);
     
     for(; rem > 0; --rem)
     {
          dst = off8(dst, -1);
          src = off8(src, -1);
          in8(dst) ^= in8(src);
     }
     for(; times > 0; --times)
     {
          dst = off64(dst, -1);
          src = off64(src, -1);
          in64(dst) ^= in64(src);
     }
}
/* 
     memxor in reverse; triggered by mem overlap
                                                   */
void memxor(void* dst, void* src, u64 len)
{
     u64 diff = ptrdiff(dst, src);
     u32 times = len >> 3;
     u8 rem = len & 7;
     
     if (diff < len)
     {
          if (src < dst)
          {
               memxor_rev(dst, src, len);
               return;
          }
     }
     
     for(; times > 0; --times)
     {
          in64(dst) ^= in64(src);
          dst = off64(dst, 1);
          src = off64(src, 1);
     }
     for(; rem > 0; --rem)
     {
          in8(dst) ^= in8(src);
          dst = off8(dst, 1);
          src = off8(src, 1);
     }
}


