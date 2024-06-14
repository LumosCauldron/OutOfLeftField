// "To God be the Glory Honor Praise Forever and Ever"

#ifndef GodIsWithMe
#define GodIsWithMe

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
     #include "mymath.h"
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
   #include "printf.h"
   #define say(format, ...) record(format, __VA_ARGS__)
   
#else
   #include <stdio.h>
   #define say(format, ...) printf(format, __VA_ARGS__)
#endif

#ifdef DEBUG
     u64 msgPushOutCount = 1;
     u8 debug_check(u64 x, char* msg, char* function)
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
                    while (pushout) { say("  "); --pushout; }
                    say("[ %s ]\n", function);
                    ++msgPushOutCount;
               }
               return 0;
          }
     }

     #define nope_msg(val, msg) debug_check(u64c((val)), (msg), (__FUNCTION__))
     #define nope_ret(val) if (!debug_check(u64c((val)), nullptr, (__FUNCTION__))) { return; }
     #define nope_msg_ret(val, msg) if (!debug_check(u64c((val)), (msg), (__FUNCTION__))) { return; }
     #define nope_retx(val, x) if (!debug_check(u64c((val)), nullptr, (__FUNCTION__))) { return (x); }
     #define nope_msg_retx(val, msg, x) if (!debug_check(u64c((val)), (msg), (__FUNCTION__))) { return (x); }
     
     /* 
          helpful
                    */
     #define nope_ptr(ptr) nope_msg_ret((ptr), "empty pointer found")
     #define nope_ptr_ret0(ptr) nope_msg_retx((ptr), "empty pointer found", 0)
     #define nope_expr(expr) nope_msg_ret((expr), "falsehood found")
     #define nope_expr_ret0(expr) nope_msg_retx((expr), "falsehood found", 0)
#else
     #define nope_msg(val, msg) 
     #define nope_ret(val) 
     #define nope_msg_ret(val, msg) 
     #define nope_retx(val, x) 
     #define nope_msg_retx(val, msg, x)
     #define nope_ptr(ptr)
     #define nope_ptr_ret0(ptr)
     #define nope_expr(expr)
     #define nope_expr_ret0(expr)
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
// ---------------------------------------------------------------------------------||  CRYPTO PLUGIN
// ---------------------------------------------------------------------------------|
#include "cipher7.h"

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  DYNAMIC MEMORY
// ---------------------------------------------------------------------------------|

/*   
     use regular 
        or
     custom allocator           
                        */
#ifdef GRACE
   #include "grace.h"
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
   #define finished printf("==================\nALLOC COUNT %lu\nFREED COUNT %lu\n==================\n\n", numallocs, numfrees); return 0
#else
   #define allocPlusPlus(x) 
   #define reallocPlusPlus(ptr) 
   #define freedPlusPlus(x) 
   #define finished return 0
#endif


// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  CUSTOM STRING
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
   return ((getlen(b) == 1) && (str_index(b, 0) == 0));
}

/*
     string operations
                         */

/*
     all appends need to be freed
                                    */
str* str_append(str* mainstr, str* toadd)
{
   nope_msg_retx(mainstr, "empty 'mainstr' pointer found", mainstr);
   nope_msg_retx(mainstr, "empty 'toadd' pointer found", mainstr);
   
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
   nope_msg_retx(dest, "empty 'dest' pointer found", dest);
   nope_msg_retx(src, "empty 'src' pointer found", dest);
   
   memto(getarray(dest), getarray(src), getlen(src));
   return dest;
}

stinl u8 str_eq(str* one, str* two)
{
   nope_msg_retx(dest, "empty 'one' pointer found", dest);
   nope_msg_retx(src, "empty 'two' pointer found", dest);
   
   if (getlen(one) != getlen(two))
   {
     return 0;
   }
   return memeq(getarray(one), getarray(two), getlen(one));
}

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

stinl u64 str_index(str* b, u64 i)
{
   nope_ptr_ret0(b);
   nope_msg_retx(i < getlen(b), "index i is larger than str length", 0);
   
   switch (typeofstr(b))
   {
      case t1 : return in8off(b, i);
      case t8 : return in64off(b, i);
      case t2 : return in16off(b, i);
      case t4 : return in32off(b, i);
      default : return in8off(b, i);
   }
}

stinl void str_init(str* b, u8 c)
{
   nope_ptr(b);
   mempaint(getarray(b), getlen(b), c);
}

// ---------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------||  TEST
// ---------------------------------------------------------------------------------|

/*
     test helpers
                    */
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
          u8 x = str_index(b->array, i);
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
str* str_w_As(u64 len)
{
     str* b = hbuffer(len);
     str_init(b, 'a');
     return b;
}
str* str_w_Cs(u64 len)
{
     str* b = hbuffer(len);
     str_init(b, 'c');
     return b;
}

int main() 
{
     str* b = str_w_As(2000);
     str* k = str_w_Cs(49);
     
     ShadowOfTheHisWing mixer5000;
     cryptokey(k->array, &mixer5000);
     ShadowOfTheHisWing mixer5001;
     cryptokey(k->array, &mixer5001);
     
     str_print_bin(b);divider();
     thyreos(b->array, getlen(b), &mixer5000);
     str_print_bin(b);divider();
     thyreos(b->array, getlen(b), &mixer5001);
     str_print_bin(b);
     
}

#endif // GodIsWithMe

// "To God be the Glory Honor Praise Forever and Ever" 
