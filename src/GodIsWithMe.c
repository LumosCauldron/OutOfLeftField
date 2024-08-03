// "To God be the Glory Honor Praise Forever and Ever"

#ifndef GodIsWithMe
#define GodIsWithMe

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  PREDEFINES 
// ---------------------------------------------------------------------------------|

     #define _GNU_SOURCE // for other 'O_' file io options
     #define _FILE_OFFSET_BITS 64 // for O_LARGEFILE file io option

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  CUSTOM TYPES
// ---------------------------------------------------------------------------------|

/* 
     Ladies and Gentlemen...
     
          types.
                               */

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ i8;
#endif
#ifdef __INT16_TYPE__
typedef __INT16_TYPE__ i16;
#endif
#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ i32;
#endif
#ifdef __INT64_TYPE__
typedef __INT64_TYPE__ i64;
#endif
#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ u8;
#endif
#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__ u16;
#endif
#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ u32;
#endif
#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ u64;
#endif

#define cast(variable, type) ( (type)(variable) )

#define u64c(variable) ( cast((variable), u64) )
#define u32c(variable) ( cast((variable), u32) )
#define u16c(variable) ( cast((variable), u16) )
#define  u8c(variable) ( cast((variable),  u8) )
#define i64c(variable) ( cast((variable), i64) )
#define i32c(variable) ( cast((variable), i32) )
#define i16c(variable) ( cast((variable), i16) )
#define  i8c(variable) ( cast((variable),  i8) )

#define u64p(variable) ( cast((variable), u64*) )
#define u32p(variable) ( cast((variable), u32*) )
#define u16p(variable) ( cast((variable), u16*) )
#define  u8p(variable) ( cast((variable),  u8*) )
#define i64p(variable) ( cast((variable), i64*) )
#define i32p(variable) ( cast((variable), i32*) )
#define i16p(variable) ( cast((variable), i16*) )
#define  i8p(variable) ( cast((variable),  i8*) )

#define nullptr (void*)(0)


// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  OFFSET INDEX
// ---------------------------------------------------------------------------------|

/*
     offseting pointers
                             */
#define off64(ptr, offset) ( u64p((ptr)) + (offset) )
#define off32(ptr, offset) ( u32p((ptr)) + (offset) )
#define off16(ptr, offset) ( u16p((ptr)) + (offset) )
#define  off8(ptr, offset) (  u8p((ptr)) + (offset) )

/*
     indexing
                */
#define in64off(ptr, offset) ( *( off64((ptr), (offset)) ) )
#define in32off(ptr, offset) ( *( off32((ptr), (offset)) ) )
#define in16off(ptr, offset) ( *( off16((ptr), (offset)) ) )
#define  in8off(ptr, offset) ( *(  off8((ptr), (offset)) ) )

#define in64(ptr) ( *( off64((ptr), 0) ) )
#define in32(ptr) ( *( off32((ptr), 0) ) )
#define in16(ptr) ( *( off16((ptr), 0) ) )
#define  in8(ptr) ( *(  off8((ptr), 0) ) )

#define invoidptr(x) (*(cast((x), void**)))

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  MATH
// ---------------------------------------------------------------------------------|

#ifdef MYMATH
     #include "include/one/mymath.h"
#else
     #include <math.h>
#endif

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  PROGRAM SWITCHES
// ---------------------------------------------------------------------------------|

/* 
     program switches
                        */
#ifdef INLINE
   #define stinl static inline __attribute__((always_inline))
#else
   #define stinl 
#endif

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  PRINT SHOP
// ---------------------------------------------------------------------------------|
#ifdef CONSOLE
   #include "include/one/printf.h"
   #define say(format, ...) record((format), __VA_ARGS__)
#elif PRINTF
   #include <stdio.h>
   #define say(format, ...) printf((format), __VA_ARGS__)
#else
   #define say(format, ...)
#endif

#ifdef DEBUG
     #include <errno.h>
     
     #define debug(...) say(__VA_ARGS__)
     
     u64 msgPushOutCount = 1;
     u8 debug_check(u64 x, char* msg, const char* function)
     {
          if (x)
          {
               return 1;
          }
          else
          {
               if (msg)
               {
                    say("%s\n", msg);
                    u64 pushout = msgPushOutCount;
                    while (pushout) { say("%s", "  "); --pushout; }
                    say("+ %s +\n", function);
                    ++msgPushOutCount;
               }
               return 0;
          }
     }

     #define nope_msg(val, msg)              debug_check(u64c((val)), (msg),   (__FUNCTION__))
     #define nope_ret(val)              if (!debug_check(u64c((val)), nullptr, (__FUNCTION__))) { return; }
     #define nope_msg_ret(val, msg)     if (!debug_check(u64c((val)), (msg),   (__FUNCTION__))) { return; }
     #define nope_retx(val, x)          if (!debug_check(u64c((val)), nullptr, (__FUNCTION__))) { return (x); }
     #define nope_msg_retx(val, msg, x) if (!debug_check(u64c((val)), (msg),   (__FUNCTION__))) { return (x); }
     
     /* 
          helpful
                    */
     #define nope_ptr(ptr)           nope_msg_ret((ptr),   "empty pointer ["#ptr"] found")
     #define nope_ptr_ret0(ptr)      nope_msg_retx((ptr),  "empty pointer ["#ptr"] found", 0)
     #define nope_ptr_retx(ptr, x)   nope_msg_retx((ptr),  "empty pointer ["#ptr"] found", (x))
     #define nope_expr(expr)         nope_msg_ret((expr),  "falsehood ["#expr"] found")
     #define nope_expr_ret0(expr)    nope_msg_retx((expr), "falsehood ["#expr"] found", 0)
     #define nope_expr_retx(expr, x) nope_msg_retx((expr), "falsehood ["#expr"] found", (x))
     
     #ifdef YANG
         #define retWindowsError(expr) \
         char buffer[8] = {0,0,0,0,0,0,0,0}; itoa(GetLastError(), buffer, 10); \
         nope_msg_ret((expr), buffer);
 
         #define retWindowsError0(expr) \
         char buffer[8] = {0,0,0,0,0,0,0,0}; itoa(GetLastError(), buffer, 10); \
         nope_msg_retx((expr), buffer, 0);
         
         #define retWindowsErrorX(expr, x) \
         char buffer[8] = {0,0,0,0,0,0,0,0}; itoa(GetLastError(), buffer, 10); \
         nope_msg_retx((expr), buffer, (x));
     #endif
#else
     #define debug(...)
     #define nope_msg(val, msg) 
     #define nope_ret(val) 
     #define nope_msg_ret(val, msg) 
     #define nope_retx(val, x) 
     #define nope_msg_retx(val, msg, x)
     #define nope_ptr(ptr)
     #define nope_ptr_ret0(ptr)
     #define nope_ptr_retx(ptr, x)
     #define nope_expr(expr)
     #define nope_expr_ret0(expr)
     #define nope_expr_retx(ptr, x)
     #define errno 0 // default errno value
     
     #ifdef YANG
         #define retWindowsError(expr)
         #define retWindowsError0(expr)
         #define retWindowsErrorX(expr, x)
     #endif
#endif

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  MEM OPERATIONS
// ---------------------------------------------------------------------------------|

/* 
     absolute value
                      */
stinl u64 pos(i64 num)
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
stinl u64 ptrdiff(void* one, void* two)
{
     nope_ptr_retx(one, u64c(two));
     nope_ptr_retx(two, u64c(one));
     return pos(u64c(one) - u64c(two));
}

/* 
     memmove in reverse; triggered by mem overlap
                                                    */
void memto_rev(void* dst, void* src, u64 len)
{    
     u32 times = len >> 3;
     u8 rem = len & u64c(7);
     
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
     nope_ptr(dst);
     nope_ptr(src);

     u64 diff = ptrdiff(dst, src);
     u32 times = len >> 3;
     u8 rem = len & u64c(7);
     
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
     u8 rem = len & u64c(7);
     
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
     nope_ptr_ret0(dst);
     nope_ptr_ret0(src);

     u64 diff = ptrdiff(dst, src);
     u32 times = len >> 3;
     u8 rem = len & u64c(7);
     
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
     nope_ptr(dst);

     u8 src[8] = { c, c, c, c, c, c, c, c };
     u32 times = len >> 3;
     u8 rem = len & u64c(7);
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
     exact grep 
                  */
void* memsearch(void* data, u64 dlen, void* snippet, u64 slen)
{    
     nope_ptr_ret0(data);
     nope_ptr_ret0(snippet);
     
     if (slen > dlen || !slen)
     {
          return nullptr;
     }
     
     dlen -= slen;
     
     do
     {
          if (memeq(data, snippet, slen))
          {
               return data;
          }
          data = off8(data, 1);
     } while (dlen--); // keep post decrement
}

/* 
     data search from one buffer to another
                                             */
void* memsearch2(void* data1, u64 dlen1, void* data2, u64 dlen2, void* snippet, u64 slen)
{
     // at least one points to data
     nope_expr_ret0(data1 != nullptr || data2 != nullptr); 
     nope_ptr_ret0(snippet);

     if (!slen)
     {
          return nullptr;
     }
     
     void* found = nullptr;
     if (dlen1 >= slen)
     {
          if (found = memsearch(data1, dlen1, snippet, slen))
          {
               return found; // reusing pointers
          }
     }
     
     if (dlen1 + dlen2 >= slen)
     {
          u64 subsnip = slen - 1;
          u64 checklen = subsnip << 1;
          u64 tempoff = subsnip;
          
          u8 temp[checklen];
          
          if (dlen1 >= subsnip) // catches case where (dlen1 + dlen2) is enough but dlen1 isnt
          {
               memto(u8p(temp), data1 + (dlen1 - subsnip), subsnip);
          }
          else
          {
               memto(u8p(temp), data1, dlen1);
               checklen -= subsnip;
               checklen += dlen1;
               tempoff = dlen1;
          }
          
          if (dlen2 >= subsnip) // catches case where (dlen1 + dlen2) is enough but dlen2 isnt
          {
               memto(u8p(temp + tempoff), data2, subsnip);
          }
          else
          {
               memto(u8p(temp + tempoff), data2, dlen2);
               checklen -= subsnip;
               checklen += dlen2;
          }
          
          if (found = memsearch(temp, checklen, snippet, slen))
          {
               u64 foundoff = ptrdiff(found, temp);
               if (dlen1 < subsnip && foundoff < dlen1)
               {
                    return u8p(data1) + foundoff;
               }
               else if (foundoff < subsnip)
               {
                    return u8p(data1) + (dlen1 - subsnip) + foundoff;
               }
               else
               {
                    if (dlen1 < subsnip)
                    {
                         return u8p(data2) + (foundoff - dlen1);
                    }
                    else
                    {
                         return u8p(data2) + (dlen2 - subsnip) + (foundoff - subsnip);
                    }
               }
          }
     }
     
     if (dlen2 >= slen)
     {
          if (found = memsearch(data2, dlen2, snippet, slen))
          {
               return found; // reusing pointers
          }
     }
     
     return nullptr;
}

/* 
     memxor in reverse; triggered by mem overlap
                                                   */
void memxor_rev(void* dst, void* src, u64 len)
{
     u32 times = len >> 3;
     u8 rem = len & u64c(7);
     
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
     nope_ptr(dst);
     nope_ptr(src);

     u64 diff = ptrdiff(dst, src);
     u32 times = len >> 3;
     u8 rem = len & u64c(7);
     
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


// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  FUNCTION MODULES
// ---------------------------------------------------------------------------------|
typedef struct DataMod
{
     u64 (*fptr)(void*, u64, void*);
     void* data;
     u64 len;
     void* context;
} datamod;

#define dlc(function, data, len, context) (cast(((datamod [1]){[0] = {(function), (data), (len), (context)}}), datamod*))
#define execdlc(m)                        ((m)->fptr)((m)->data, (m)->len, (m)->context)
#define dm(function, data, len, context) execdlc(dlc((function), (data), (len), (context)))

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  CRYPTO PLUGIN
// ---------------------------------------------------------------------------------|
#include "include/one/cipher7.h"

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  DYNAMIC MEMORY
// ---------------------------------------------------------------------------------|

/*   
     use regular 
        or
     custom allocator           
                        */
#ifdef GRACE
   #include "include/one/grace.h"
   #define grace(x) allocPlusPlus((x)) _grace((x))
   #define moregrace(ptr, x) reallocPlusPlus((ptr)) _moregrace((ptr), (x))
   #define restore(x) freedPlusPlus((x)) _restore((void*)(x))
#else
   #include <stdlib.h>
   #define grace(x) allocPlusPlus((x)) malloc((x))
   #define moregrace(ptr, x) reallocPlusPlus((ptr)) realloc((ptr), (x))
   #define restore(x) freedPlusPlus((x)) free((void*)(x))
#endif

/*   
     keep track of allocs
     keep track of frees
     when in debug mode only           
                               */
#ifdef DEBUG
   u64 numfrees = 0;
   u64 numallocs = 0;
   #define allocPlusPlus(x) ((numallocs += ((x) != 0)) & 0x0000000000000000) +
   #define reallocPlusPlus(ptr) ((numallocs += (!(ptr))) & 0x0000000000000000) +
   #define freedPlusPlus(x) numfrees += ((x) != 0);
   #define finished say("==================\nALLOC COUNT %lu\nFREED COUNT %lu\n==================\n\n", numallocs, numfrees); return 0
#else
   #define allocPlusPlus(x) 
   #define reallocPlusPlus(ptr) 
   #define freedPlusPlus(x) 
   #define finished return 0
#endif

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  CUSTOM STRINGS
// ---------------------------------------------------------------------------------|

/*
     internal string type management
                                       */
#define memkind 0b00000011 // mask to get value (not a conditional value)
#define kinddata 0b00000011
#define kindstack 0b00000010
#define kindheap 0b00000001
#define clearkind 0b11111100 // mask out tool (not a conditional value)

#define memtype 0b00001100 // mask to get value
#define t8 0b00001100
#define t4 0b00001000
#define t2 0b00000100
#define t1 0b00000000
#define cleartype 0b11110011 // mask out tool

#define memevade 0b00010000 // mask to get value
#define memprotect 0b00100000 // mask to get value
#define clearevade 0b11101111 // mask out tool
#define clearprotect 0b11011111 // mask out tool

#define chardata (kinddata | t1)
#define shortdata (kinddata | t2)
#define intdata (kinddata | t4)
#define longdata (kinddata | t8)
#define charstack (kindstack | t1)
#define shortstack (kindstack | t2)
#define intstack (kindstack | t4)
#define longstack (kindstack | t8)
#define charheap (kindheap | t1)
#define shortheap (kindheap | t2)
#define intheap (kindheap | t4)
#define longheap (kindheap | t8)
#define evade(t) (memevade | (t))
#define protect(t) (memprotect | (t))
#define evadeprotect(t) (memprotect | memevade | (t))

/* 
     stacks 
              */
#define charspace(x) u8p(((char [(x) + 1]){[(x)] = 0}))
#define shortspace(x) u16p(((u16 [(x) + 1]){[(x)] = 0}))
#define intspace(x) u32p(((u32 [(x) + 1]){[(x)] = 0}))
#define longspace(x) u64p(((u64 [(x) + 1]){[(x)] = 0}))

/* 
     stack strs 
                  */
#define litstrlen(datastr) (sizeof(datastr) - 1)
#define datastr(arr) cast(((u64 [2]){[0] = tolen7(litstrlen((arr)), chardata), [1] = u64c((arr))}), str*)
#define wdatastr(arr) cast(((u64 [2]){[0] = tolen7(litstrlen((arr)) - 1, shortdata), [1] = u64c((arr))}), str*) // -1 in len  for wide nul-terminator
#define charstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), charstack), [1] = u64c((arr))}), str*)
#define shortstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), shortstack), [1] = u64c((arr))}), str*)
#define intstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), intstack), [1] = u64c((arr))}), str*)
#define longstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), longstack), [1] = u64c((arr))}), str*)
#define buffer(len) charstr((len), charspace((len))) // len must be a constant
#define shortbuffer(len) shortstr((len), shortspace((len))) // len must be a constant
#define intbuffer(len) intstr((len), intspace((len))) // len must be a constant
#define longbuffer(len) longstr((len), longspace((len))) // len must be a constant

/* 
     heap strs 
                 */
#define hcharstr(len, arr) str_buf(tolen7((len), charheap), (arr))
#define hshortstr(len, arr) str_buf(tolen7((len), shortheap), (arr))
#define hintstr(len, arr) str_buf(tolen7((len), intheap), (arr))
#define hlongstr(len, arr) str_buf(tolen7((len), longheap), (arr))
#define hbuffer(len) str_buf(tolen7((len), charheap), nullptr)
#define hshortbuffer(len) str_buf(tolen7((len), shortheap), nullptr)
#define hintbuffer(len) str_buf(tolen7((len), intheap), nullptr)
#define hlongbuffer(len) str_buf(tolen7((len), longheap), nullptr)

/*
     do not change
                     */
typedef struct bytes
{
   u64 len7;
   void* array;
} str;

/*
     prototypes
                  */
stinl str* str_buf(u64, void*);
stinl str* str_copy(str* dest, str* src);
stinl u8 get7(str*);
stinl u64 getlen(str* b);
stinl u64 str_index(str*, u64);
stinl void str_init(str*, u8);
stinl u8 typeofstr(str*);

/*
     strlen replacement
                          */
stinl u64 tilnul(char* array)
{
   u64 count = 0;
   while(*array)
   {
     ++array;
     ++count;
   }
   return count;
}                      

/*
     setters
               */
stinl u64 tolen7(u64 len, u8 b7)
{
   return (0x00ffffffffffffff & len) | (u64c(b7) << 56);
}

stinl void set7(str* b, u8 b7)
{
   nope_ptr(b);
   b->len7 = tolen7(b->len7, b7);
}

stinl void setevade(str* b, u8 e)
{
   nope_ptr(b);
   set7(b, (get7(b) & clearevade) | memevade & e);
}

stinl void setkind(str* b, u8 k)
{
   nope_ptr(b);
   set7(b, (get7(b) & clearkind) | memkind & k);
}

stinl void setprotect(str* b, u8 p)
{
   nope_ptr(b);
   set7(b, (get7(b) & clearprotect) | memprotect & p);
}

stinl void settype(str* b, u8 t)
{
   nope_ptr(b);
   set7(b, (get7(b) & cleartype) | memtype & t);
}

stinl void setlen(str* b, u64 len)
{
   nope_ptr(b);
   b->len7 = (b->len7 & 0xff00000000000000) | (0x00ffffffffffffff & len);
}

stinl void incrlen(str* b, u64 amt)
{
   nope_ptr(b);
   b->len7 = (b->len7 & 0xff00000000000000) | (0x00ffffffffffffff & (b->len7 + amt));
}

stinl void decrlen(str* b, u64 amt)
{
   nope_ptr(b);
   b->len7 = (b->len7 & 0xff00000000000000) | (0x00ffffffffffffff & (b->len7 - amt));
}

stinl void setlen7(str* b, u64 len7)
{
   nope_ptr(b);
   b->len7 = len7;
}

stinl void setarray(str* b, void* array)
{
   nope_ptr(b);
   b->array = array;
}

stinl void setarraylen7(str* b, void* array, u64 len, u8 b7)
{
   nope_ptr(b);
   setarray(b, array);
   setlen7(b, tolen7(len, b7));
}

stinl void setstr(str* src, str* dest)
{
   nope_ptr(src);
   nope_ptr(dest);
   dest->array = src->array;
   dest->len7 = src->len7;
}

/*
     getters
               */
stinl u8 get7(str* b)
{
   nope_ptr_ret0(b);
   return (b->len7 & 0xff00000000000000) >> 56;
}
stinl u8 getevade(str* b)
{
   nope_ptr_ret0(b);
   return get7(b) & memevade;
}
stinl u8 getkind(str* b)
{
   nope_ptr_ret0(b);
   return get7(b) & memkind;
}
stinl u8 getprotect(str* b)
{
   nope_ptr_ret0(b);
   return get7(b) & memprotect;
}
stinl u8 gettype(str* b)
{
   nope_ptr_ret0(b);
   return get7(b) & memtype;
}
stinl u64 lenoflen7(u64 len7)
{
   return 0x00ffffffffffffff & len7;
}
stinl u64 getlen(str* b)
{
   nope_ptr_ret0(b);
   return 0x00ffffffffffffff & b->len7;
}
stinl u64 getlenbytes(str* b)
{
   nope_ptr_ret0(b);
   switch (typeofstr(b))
   {
      case t1 : return (0x00ffffffffffffff & b->len7);
      case t8 : return (0x00ffffffffffffff & b->len7) << 3; // mult by 8
      case t2 : return (0x00ffffffffffffff & b->len7) << 1; // mult by 2
      case t4 : return (0x00ffffffffffffff & b->len7) << 2; // mult by 4
      default : return (0x00ffffffffffffff & b->len7);
   }
}
stinl u64 getlen7(str* b)
{
   nope_ptr_ret0(b);
   return b->len7;
}
stinl u8* getarray(str* b)
{
   nope_ptr_ret0(b);
   return u8p(b->array);
}

/*
     get attributes
                      */
stinl u8 typeofstr(str* b)
{
   nope_ptr_ret0(b);
   return memtype & get7(b);
}
stinl u8 kindofstr(str* b)
{
   nope_ptr_ret0(b);
   return memkind & get7(b);
}
stinl u8 evadingstr(str* b)
{
   nope_ptr_ret0(b);
   return memevade & get7(b);
}
stinl u8 protectedstr(str* b)
{
   nope_ptr_ret0(b);
   return memprotect & get7(b);
}
stinl u8 isemptystr(str* b) // is it a str with a length of 1 pointing to a nul-terminator
{
   nope_ptr_ret0(b);
   return (getlen(b) == 0);
}

/*
     string operations
                         */

/*
     all appends need to be freed
                                    */
str* str_append(str* mainstr, str* toadd)
{
   nope_ptr_retx(mainstr, mainstr);
   nope_ptr_retx(toadd, mainstr);
   
   if (!mainstr && !toadd)
   {
      return nullptr;
   }
   if (!mainstr)
   {
      str* newstr = hbuffer(getlenbytes(toadd));
      return str_copy(newstr, toadd);
   }
   if (!toadd)
   {
      return mainstr;
   }
   
   // calc lengths
   u64 mainlen = getlenbytes(mainstr);
   u64 toaddlen = getlenbytes(toadd);
   u64 bothlen = mainlen + toaddlen;
   u8* dest;
   
   // change kind if we need to morph 
   // from a data or stack into a heapstr
   if (getkind(mainstr) != kindheap)
   {
      dest = u8p(grace(bothlen + 1));
      memto(dest, getarray(mainstr), mainlen);
      memto(dest + mainlen, getarray(toadd), toaddlen);
      mainstr = hcharstr(bothlen, dest);
   }
   else
   {
      dest = u8p(moregrace(getarray(mainstr), bothlen + 1));
      memto(dest + mainlen, getarray(toadd), toaddlen);
   }

   // nul terminate
   *(dest + bothlen) = 0;
   
   // set and return
   setarray(mainstr, dest);
   setlen(mainstr, bothlen);
   
   return mainstr;
}

/*
     all returns from calls to str_buf's need to be freed 
     do not pass in literal strings
                                                            */
str* str_buf(u64 len7, void* bytes)
{
   str* newstr = grace(sizeof(str));
   if (newstr)
   {
      u64 len = lenoflen7(len7);
      newstr->len7 = len7;
      if (!bytes)
      {
         switch (typeofstr(newstr))
         {
            case t1 : newstr->array = grace(len + 1);
                      in8off(newstr->array, len) = 0; 
                      break;
            case t8 : newstr->array = grace((len + 1) << 3); // mult by 8 bytes
                      in64off(newstr->array, len) = 0; 
                      break;
            case t2 : newstr->array = grace((len + 1) << 1); // mult by 2 bytes
                      in16off(newstr->array, len) = 0; 
                      break;
            case t4 : newstr->array = grace((len + 1) << 2); // mult by 4 bytes
                      in32off(newstr->array, len) = 0; 
                      break;
            default : newstr->array = grace(len + 1);
                      in8off(newstr->array, len) = 0; 
                      break;
         }
      }
      else
      {
         newstr->array = bytes;
      }
   }
   return newstr;
}

/*
     memory for dest needs 
     to have been preallocated
                                 */
stinl str* str_copy(str* dest, str* src)
{
   nope_ptr_retx(dest, dest);
   nope_ptr_retx(src, dest);
   
   memto(getarray(dest), getarray(src), getlen(src));
   return dest;
}

/*
     string equivalence
                          */
stinl u8 str_eq(str* one, str* two)
{
   nope_ptr_ret0(one);
   nope_ptr_ret0(two);
   
   if (getlen(one) != getlen(two))
   {
     return 0;
   }
   return memeq(getarray(one), getarray(two), getlen(one));
}

/*
     free string
                   */
void str_free(void* ptr)
{
   nope_ptr(ptr);

   if (ptr)
   {
      if (invoidptr(ptr))
      {
         str* b = cast(invoidptr(ptr), str*);
         
         u8* array = getarray(b);
         u8 evade = evadingstr(b);
         u8 kind = kindofstr(b);
         if (array)
         {
            // zeroize
            if (evade && (kind == kindstack || kind == kindheap))
            {
               memscrub(b->array, getlen(b));
            }
            // check kind before freeing
            if (kind == kindheap)
            {
               restore(array);
            }
         }
         
         // zeroize
         if (evade)
         {
            evade = 0;
            kind = 0;
            array = nullptr; // set variable to zero
            b->len7 = 0;
            b->array = nullptr;  // set actual array pointer to zero
         }
         
         // check kind before freeing
         if (kind == kindheap)
         {
            restore(b);
         }
         invoidptr(ptr) = nullptr;
      }
   }
}

/*
     get string element at index
                                   */
stinl u64 str_index(str* b, u64 i)
{
   nope_ptr_ret0(b);
   nope_msg_retx(i < getlen(b), "index i is larger than str length", 0);
   
   switch (typeofstr(b))
   {
      case t1 : return in8off(b->array, i);
      case t8 : return in64off(b->array, i);
      case t2 : return in16off(b->array, i);
      case t4 : return in32off(b->array, i);
      default : return in8off(b->array, i);
   }
}

/*
     initialize string
                         */
stinl void str_init(str* b, u8 c)
{
   nope_ptr(b);
   mempaint(getarray(b), getlen(b), c);
}

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  FILE OPERATIONS
// ---------------------------------------------------------------------------------|

     // notes: 
     // add a way to move down dir/file path in case they go above 256-260 characters

#define executemode 1
#define writemode 2
#define readmode 4 
#define readwrite 6
#define specialmode 7
#define createmode 8

// read/write speed



#ifdef YANG
     #include <windows.h>
     
     /* 
          Windows Server 2003 and Windows XP:  
          Pipe write operations across a network are limited in size per write. 
          The amount varies per platform. 
          For x86 platforms it's 63.97 MB. 
          For x64 platforms it's 31.97 MB. 
          For Itanium it's 63.95 MB. 
          For more information regarding pipes, see the Remarks section 
          fOr mOrE iNfOrMaTiOn rEgArDiNg pIpEs, sEe tHe rEmArKs sEcTiOn   
                                                                                  */
     #define disk_block_size 4096
     
     #if defined(WINXP) || defined(WIN2003)
     
          #define fast_aligned_rate (1024 * 1024 * 30) // 30 megabytes per read
          
     #elif defined EMBED
     
          #define fast_aligned_rate (disk_block_size) // 4 kilobytes per read
     #else
     
          #define fast_aligned_rate ((u64)(0b00000001 << 30)) // ~2147 megabytes per read
     
     #endif
     
     #define rwDBS disk_block_size
     #define rwFAR fast_aligned_rate
     
     typedef HANDLE STREAM;
     typedef int MODE;
     
     #define read_failed -1
     #define write_failed -1
     #define seek_failed -1
     #define invalid_stream ((HANDLE)(LONG_PTR)-1)
     
     #define max_path_supported 260 // avoids multiple syscalls
     
     /*   
          windows file ops   
                            */
     stinl u8 filepath_check(str* filename)
     {  
        nope_ptr_ret0(filename);
        nope_expr_ret0(!isemptystr(filename));
        nope_expr_ret0(getlen(filename) <= max_path_supported);
        return 1;
     }

     STREAM file_open(str* filename, MODE access) 
     {  
         nope_expr_retx(filepath_check(filename), invalid_stream);
         
         u32 generic_access;
         u32 how_to_open;
         switch (access)
         {
            case readmode    : ; generic_access = GENERIC_READ;
                                 how_to_open = OPEN_EXISTING;
                         break;
            case writemode   : ; generic_access = GENERIC_WRITE;
                                 how_to_open = OPEN_EXISTING;
                         break;
            case specialmode : ; // fall into readwrite for permissions
            case readwrite   : ; generic_access = GENERIC_READ | GENERIC_WRITE;
                                 how_to_open = OPEN_EXISTING;
                         break;
            case executemode : ; generic_access = GENERIC_EXECUTE;
                                 how_to_open = OPEN_EXISTING;
                         break;
            case createmode  : ; generic_access = GENERIC_WRITE;
                                 how_to_open = CREATE_ALWAYS;
                         break;
            default : ;  nope_msg_retx(0, "unknown mode specified", invalid_stream);
                         break;
         }

         STREAM stream = CreateFileA(filename->array, 
                                     generic_access, 
                                     0, NULL, 
                                     how_to_open, 
                                     FILE_FLAG_NO_BUFFERING  | 
                                     FILE_FLAG_WRITE_THROUGH | 
                                     FILE_ATTRIBUTE_HIDDEN, 
                                     NULL);

         retWindowsErrorX(stream != invalid_stream, invalid_stream);

             // debugging
             #ifdef DEBUG
                  if (access != createmode)
                  {
                     debug("%s : %s\n", "open called", getarray(filename));
                  }
                  else
                  {
                     debug("%s : %s\n", "create called", getarray(filename));
                  }
               
                  debug("%s : %d\n", "fd given", stream);
             #endif

         return stream;
     }

     void file_close(STREAM stream)
     {
        nope_expr(stream != invalid_stream);
        u8 if_handle_not_closed = CloseHandle(stream);
        retWindowsError(if_handle_not_closed);
        debug("fd closed : %ld\n", stream);
     }

     u8 file_exists(str* filename)
     {
        nope_expr_ret0(filepath_check(filename));
        u32 attributes = GetFileAttributes(filename->array);
        return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
     }
     
     u64 file_seek(STREAM in, u64 offset, u32 mode)
     {
          LARGE_INTEGER amount = {0};
          LARGE_INTEGER newPos = {0};
          amount.QuadPart = i64c(offset);
          
          u8 if_file_pointer_not_set = SetFilePointerEx(in, amount, &newPos, mode);
          retWindowsError0(if_file_pointer_not_set);
          
          return u64c(newPos.QuadPart);
     }
     
     stinl u64 file_position(STREAM in)
     {
          return file_seek(in, 0, FILE_CURRENT);
     }

     u64 file_size(str* filename) // start here
     {
          nope_expr_ret0(filepath_check(filename));

          WIN32_FILE_ATTRIBUTE_DATA file_info;
          if (GetFileAttributesExA(filename->array, GetFileExInfoStandard, &file_info))
          {
             return u64c(file_info.nFileSizeLow) | (u64c(file_info.nFileSizeHigh) << 32);
          }

          retWindowsError0(0); // there has been a windows error at this point
     }

     u8 file_destroy(str* filename)
     {
        nope_expr_ret0(filepath_check(filename));
        
        u8 if_file_not_deleted = DeleteFileA(getarray(filename));
        retWindowsError0(if_file_not_deleted != 0);
        
        debug("%s -/-> %s\n", "DeleteFileA called", getarray(filename));
        
        return 1;
     }
     
     int set_file_size(STREAM in, u64 len)
     {
          // save original file position
          u64 origin_pos = file_position(in);
          
          // map out file size by moving pointer to desired distance from beginning
          file_seek(in, len, FILE_BEGIN);
          
          // set the file boundary to where file pointer is
          u8 if_end_of_file_not_set = SetEndOfFile(in);
          retWindowsErrorX(if_end_of_file_not_set, seek_failed);
          
          // reset file pointer to original position
          file_seek(in, origin_pos, FILE_BEGIN);
          
          return 0;
     }
     
     int stream_read(STREAM in, void* data, u64 readrate)
     {
          DWORD bytes_read;

          u8 if_bad_read = ReadFile(in, data, readrate, &bytes_read, 0);
          retWindowsErrorX(if_bad_read, read_failed);
          
          return 0;
     }
     
     int stream_write(STREAM out, void* data, u64 readrate)
     {
          DWORD bytes_written;

          u8 if_bad_write = WriteFile(out, data, readrate, &bytes_written, nullptr);
          retWindowsErrorX(if_bad_write, write_failed);
          
          return 0;
     }
       
#else // YIN

     #define disk_block_size 4096
     
     #ifdef EMBED
     
          #define fast_aligned_rate (disk_block_size) // 4 kilobytes per read
          
     #else
     
          #define fast_aligned_rate ((u64)(0b00000001 << 30)) // ~2147 megabytes per read
     
     #endif
     
     #define rwDBS disk_block_size
     #define rwFAR fast_aligned_rate
     
     // their libs
     #include <fcntl.h>
     #include <sys/types.h>
     #include <sys/stat.h>
     #include <unistd.h>
     
     typedef i64 STREAM;
     typedef int MODE;

     #define invalid_stream -1
     
     #define read_failed -1
     #define seek_failed -1
     #define stat_failed -1
     #define unlink_failed -1
     #define write_failed -1
     
     #define slashstr "/"
     
     #define file_seek(...) lseek(__VA_ARGS__)
     #define stream_read(...) read(__VA_ARGS__)
     #define stream_write(...) write(__VA_ARGS__)
     #define set_file_size(...) ftruncate(__VA_ARGS__)

     // if not OSX/iOS (IZANAMI)
     #ifndef O_DIRECT
        #define O_DIRECT 0 
     #endif

     u64 max_path_supported = 0;
     
     /*   
          linux file ops   
                          */
     
     stinl u8 filepath_check(str* filename)
     {
        if (!max_path_supported)
        {
           max_path_supported = pathconf(slashstr, _PC_PATH_MAX);
        }
        
        nope_ptr_ret0(filename);
        nope_expr_ret0(!isemptystr(filename));
        nope_expr_ret0(getlen(filename) <= max_path_supported);
        
        return 1;
     }

     STREAM file_open(str* filename, MODE access) 
     {
        nope_expr_retx(filepath_check(filename), invalid_stream);
        
        // =-> (write straight to disk linux:"O_DIRECT", osx:"fcntl(fd, F_NOCACHE, 1)"
        // =-> large files ok
        // =-> don't update date
        // =-> symlinks don't count
        // =-> ensure data don't care about metadata
        int genflags = O_LARGEFILE | O_NOATIME  | 
                      O_NOFOLLOW  | O_DSYNC    | O_DIRECT;
        
        // =-> when creating/createmode: replace already existing, write only then add the above
        int finalflags = O_CREAT | O_WRONLY | 
                       O_TRUNC | genflags;
                               
        int mode = S_ISVTX | S_IRWXU;
        switch (access)
        {
           case createmode: 
                 break;
           case specialmode: // =-> fall into readwrite for permissions
           case readwrite: finalflags = genflags | O_RDWR; // =-> what we want to do
                           mode = S_IRUSR | S_IWUSR; // =-> what we need to do it
                           break;
                           
           case readmode: finalflags = genflags | O_RDONLY;
                          mode = S_IRUSR; 
                          break;
                          
           case executemode: finalflags = genflags | O_RDONLY;
                             mode = S_IRUSR | S_IXUSR; 
                             break;
                             
           case writemode: finalflags = genflags | O_WRONLY;
                           mode = S_IWUSR; 
                           break;
           default: nope_msg_retx(0, "unknown mode specified", invalid_stream);
        }
        
        // actual open operation
        int fd = open(getarray(filename), finalflags, mode);
        
        // debugging
        #ifdef DEBUG
             if (access != createmode)
             {
                debug("%s : %s\n", "open called", getarray(filename));
             }
             else
             {
                debug("%s : %s\n", "create called", getarray(filename));
             }
          
             debug("%s : %d\n", "fd given", fd);
             debug("%s : %ld\n", "error", errno);
        #endif
        
        nope_expr_retx(fd != invalid_stream, invalid_stream);
        
        #ifdef IZANAMI
           if (fd != invalid_stream)  // same as using O_DIRECT but for OSX
           {
              fcntl(fd, F_NOCACHE, 1);
           }
        #endif
        
        return fd;
     }

     void file_close(STREAM stream)
     {
        nope_expr(stream >= 0);
        
        if (stream >= 0)
        {
           close(stream);
           debug("fd closed : %ld\n", stream);
        }
     }
     
     u64 file_position(STREAM in)
     {
          i64 position = file_seek(in, 0, SEEK_CUR);
          nope_expr_ret0(position != seek_failed);
          return u64c(position);
     }

     u8 file_exists(str* filename)
     {
        nope_ptr_ret0(filename); 
        struct stat info;
        int retval = stat(getarray(filename), &info);
        return (retval == 0);
     }

     u64 file_size(str* filename)
     {
        nope_ptr_ret0(filename); 
        struct stat info;
        int retval = stat(getarray(filename), &info);
        nope_expr_ret0(retval != stat_failed);
        return info.st_size;
     }

     u8 file_destroy(str* filename)
     {
        nope_expr_ret0(filepath_check(filename));
        
        // =-> add support for file overwriting
        // =-> if (paranoid)
        // =-> file_scrub()...

        // =-> destroy link to the file 
        int success = unlink(getarray(filename));
        
        debug("%s -/-> %s\n", "unlink called", getarray(filename));
        debug("%s : %d\n", "error", errno);
        
        // =-> if unlinking failed
        nope_expr_ret0(success != unlink_failed);
        
        return 1;
     }
     
#endif // YIN

stinl u8 readfilef(STREAM in, u64 amt, STREAM out, Shield* faith, u64 (*fptr)(void*, u64, void*), void* context)
{
   u64 readrate = (amt / rwDBS) * rwDBS;
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
   u8* data = s->array;
   u64 times = amt / readrate; 
   u16 remainder = amt % readrate;
   i16 retval = 0;
   
   while (times)
   {
      retval = stream_read(in, data, readrate);
      nope_expr_ret0(retval != read_failed);
      
      // check if processing of data is needed
      // 'fptr' and 'context' may be passed in here
      if (fptr)
      {
          dm(fptr, data, readrate, context);
      }
      
      // check if encryption is desired
      // 'faith' may be passed in here
      if (faith)
      {
         encrypt_string(data, readrate, faith);
      }
      
      // check that descriptors are not the same
      // if they are, writing is not desired
      if (in != out)
      {
          retval = stream_write(out, data, readrate);
          nope_expr_ret0(retval != write_failed);
      }
      
      --times;
   }
   
   if (remainder)
   {
      retval = stream_read(in, data, readrate);
      nope_expr_ret0(retval != read_failed);
      
      // check if processing of data is needed
      // 'fptr' and 'context' may be passed in here
      if (fptr)
      {
          dm(fptr, data, remainder, context);
      }
      
      // check if encryption is desired
      // 'faith' may be passed in here
      if (faith)
      {
         encrypt_string(data, remainder, faith);
      }
      
      // check that descriptors are not the same
      // if they are, writing is not desired
      if (in != out)
      {
           retval = stream_write(out, data, readrate);
           nope_expr_ret0(retval != write_failed);
      }
   }
   
   str_free(&s);
   return 1;
}

stinl u8 readfileb(STREAM in, u64 amt, STREAM out, Shield* faith, u64 (*fptr)(void*, u64, void*), void* context)
{
   u64 readrate = (amt / rwDBS) * rwDBS;
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
   u64 times = (amt / readrate); 
   i64 backup = -(readrate << 1); // keep a signed integer
   u16 remainder = amt % readrate;
   i16 retval = 0;
   

   // calibrate first read (its ok if remainder is 0)
   if (remainder)
   {
      retval = file_seek(in, -remainder, SEEK_END);
      nope_expr_ret0(retval != seek_failed);
      retval = file_seek(out, -remainder, SEEK_END);
      nope_expr_ret0(retval != seek_failed);
   }
   else
   {
      retval = file_seek(in, -readrate, SEEK_END); 
      nope_expr_ret0(retval != seek_failed);
      retval = file_seek(out, -readrate, SEEK_END);
      nope_expr_ret0(retval != seek_failed);
   }
         
   do
   {
      retval = stream_read(in, data, readrate);
      nope_expr_ret0(retval != read_failed);
      
      // check if processing of data is needed
      // 'fptr' and 'context' may be passed in here
      if (fptr)
      {
          if (remainder)
          {
               dm(fptr, data, remainder, context);
          }
          else
          {
               dm(fptr, data, readrate, context);
          }
      }
      
      // check if encryption is desired
      // 'faith' may be passed in here
      if (faith)
      {
         if (remainder)
         {
            encrypt_string(data, remainder, faith);
         }
         else
         {
            encrypt_string(data, readrate, faith);
         }
      }
      
      // check that descriptors are not the same
      // if they are, writing is not desired
      if (in != out)
      {
           retval = stream_write(out, data, readrate);
           nope_expr_ret0(retval != write_failed);
      }
      
      // meets 2 conditions: when only remainder exists OR last time
      // this if-statement is the wall to make sure file_seek doesn't place us before file
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
         retval = file_seek(in, backup + cast(readrate - remainder, i64), SEEK_CUR);
         nope_expr_ret0(retval != seek_failed);
      }
      else
      {
         retval = file_seek(in, backup, SEEK_CUR);
         nope_expr_ret0(retval != seek_failed);
      }
      retval = file_seek(out, backup, SEEK_CUR);
      nope_expr_ret0(retval != seek_failed);
      
      // "turn off" the remainder
      remainder = 0;
      --times;
   } while (1);
   
   str_free(&s);
   return 1;
}

#define decryptfile(oldf, newf, faith) encryptfile((oldf), (newf), (faith))
void encryptfile(str* oldf, str* newf, Shield* faith)
{
   u64 filesz = file_size(oldf);
   nope_expr(filesz > 0);
   
   STREAM in = file_open(oldf, readmode);
   STREAM out = file_open(newf, createmode);
   
   set_file_size(out, filesz); 
   if (readfilef(in, filesz, out, faith, nullptr, nullptr))
   {
      set_file_size(out, filesz); // corrects the overwriting of direct-disk-writes
      debug("read [%s] successfully (%lu bytes)\n", oldf->array, filesz);
      debug("wrote [%s] successfully (%lu bytes)\n", newf->array, filesz);
   }
   else
   {
      debug("FAILED to read  [%s] fully\n", oldf->array);
      debug("FAILED to write [%s] fully\n", newf->array);
   }
   
   file_close(in);
   file_close(out);
}

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  VECTORS
// ---------------------------------------------------------------------------------|

#define VECTOR_GROW_RATE 77
#define vect_top(v)  (cast(v->head->array, void*))
#define vect_back(v) (cast(off64(v->head->array, v->len), void*))
typedef struct Vect
{
     str* head;
     u64 len;
} vect;
void vect_create()
{
     vect* v = cast(grace(sizeof(vect)), vect*);
     v->head = longbuffer(VECTOR_GROW_RATE);
     v->len = 0;
}
void vect_destoy(vect** vaddr)
{
     str_free(&(*vaddr)->head);
     (*vaddr)->len = 0;
     restore(*vaddr);
     *vaddr = nullptr;
}
void vect_push(vect* v, void* data)
{
     
}
void vect_pop()
{
}
void vect_pop_range()
{
}


// ====================================================
// File Encryption Main
// ====================================================
int main(int numparams, char** params)
{  
   // check enough parameters exist
   if (numparams < 3)
   {
     return 1;
   }

   // file path length
   u32 filename_length = tilnul(params[1]);
   
   // file path to encrypt
   str* filename = charstr(filename_length, params[1]);

   // check it exists
   if (!file_exists(filename))
   {
     return 2;
   }
   
   // setup encrypted file name
   u8 name_holder[filename_length + 4];
   memto(name_holder, filename->array, filename_length);
   
   if (filename_length > 3)
   {
     if (name_holder[filename_length - 1] == '7' && 
         name_holder[filename_length - 2] == 'c' &&
         name_holder[filename_length - 3] == '.')
     {
          name_holder[filename_length - 3] = 0;
          filename_length -= 3;
          goto naming_done;
     }

   }
     
     name_holder[  filename_length  ] = '.';
     name_holder[filename_length + 1] = 'c';
     name_holder[filename_length + 2] = '7';
     name_holder[filename_length + 3] = 0; // nul-terminate
     filename_length += 3;
   
   naming_done: ;
   str* encrypted_filename = charstr(filename_length, name_holder);
   
   // setup key
   u64 keylen = tilnul(params[2]);
   u8 key[MSGBLKSZ + 1];
   mempaint(key, MSGBLKSZ, 7);
   keylayer(key, params[2], keylen);
   key[MSGBLKSZ] = 0;

   // setup encryption context
   Shield faith;
   encryption_initialize(key, &faith);
   
   // encrypt
   encryptfile(filename, encrypted_filename, &faith);
   
   finished;
}


// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  TEST
// ---------------------------------------------------------------------------------|

/*
     test helpers
                    
void divider()
{
     say("%s\n", "-----------------------------");
}
void str_print(str* b)
{
     say("%s\n", b->array);
}
void str_print_bin(str* b)
{
     u64 amt = getlen(b);
     u64 i = 0;
     while(amt)
     {
          if (i % 20 == 0 && i)
          {
               say("%c", '\n');
          }
          u8 x = str_index(b, i);
          if (x <= 0xf)
          {
               say("0%x ", x);
          }
          else
          {
               say("%x ", x);
          }
          ++i;
          --amt;
     }
     say("%c", '\n');
}
str* str_w_letter(u64 len, u8 c)
{
     str* b = hbuffer(len);
     str_init(b, c);
     return b;
}

int main()
{
     str* one = str_w_letter(2, 'x');
     str* two = str_w_letter(3, 'y');
     
     u8* found = u8p(memsearch2(one->array, getlen(one), two->array, getlen(two), "xxyyy", 5 ));
     if (found)
     {
          say("%s\n", found);
     } 
     else
     {
          say("%s\n", "not found");
     } 
     return 0;
}

*/



















#endif // GodIsWithMe

// "To God be the Glory Honor Praise Forever and Ever" 
