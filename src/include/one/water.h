#include "viscosity.h" // Above all declarations

#ifndef __TAD
#define __TAD

// let there be types &&&&&&&&&&&&&&&&&&&&&&&&&&
#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
#endif
#ifdef __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
#endif
#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
#endif
#ifdef __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
#endif
#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
#endif
#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__ uint16_t;
#endif
#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#endif
#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#endif
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

// allocation LOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOC
#ifdef USEMYMEMOPS
   #include "alloc.h"
#else
   #include <stdlib.h>
#endif // USEMYMEMOPS

// Don't worry, weird syntax is taken care of in the PRINTDEBUG 
// section (we still get the correct pointer assigned)
#define give(x) allocPlusPlus((x)) malloc((x))
#define regive(ptr, x) reallocPlusPlus((ptr)) realloc((ptr), (x))
#define givewhite(x) allocPlusPlus((x)) calloc((x), 1)
#define restore(x) freedPlusPlus((x)) free((void*)(x))

// LOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOCLOC

// debugging ********************************************************
#ifdef PRINTDEBUG
   #define assertret(expr, msg) if (!(expr)) { sayline(msg); return; }
   #define assertret0(expr, msg) if (!(expr)) { sayline(msg); return 0; }
   #define assertretx(expr, msg, x) if (!(expr)) { sayline(msg); return (x); }
   
   #include <errno.h>
   #define asserterrno(expr, msg) if (!(expr)) { perror(#msg); return; }
   #define asserterrnoret0(expr, msg) if (!(expr)) { perror(#msg); return 0; }
   #define asserterrnoretx(expr, msg, x) if (!(expr)) { perror(#msg); return (x); }
#else
   #define assertret(expr, msg) if (!(expr)) { return; }
   #define assertret0(expr, msg) if (!(expr)) { return 0; }
   #define assertretx(expr, msg, x) if (!(expr)) { return (x); }
   #define asserterrno(expr, msg) if (!(expr)) { return; }
   #define asserterrnoret0(expr, msg) if (!(expr)) { return 0; }
   #define asserterrnoretx(expr, msg, x) if (!(expr)) { return (x); }
#endif
// ******************************************************************

// mem attributes mmmmmmmmmmmmmmmm
#define memkind 0b00000011 // mask to get value (not a conditional value)
#define kinddata 0b00000011
#define kindstack 0b00000010
#define kindheap 0b00000001
#define clearkind 0b11111100 // mask out tool (not a conditional value)
//
#define memtype 0b00001100 // mask to get value
#define t8 0b00001100
#define t4 0b00001000
#define t2 0b00000100
#define t1 0b00000000
#define cleartype 0b11110011 // mask out tool
//
#define memevade 0b00010000 // mask to get value
#define memprotect 0b00100000 // mask to get value
#define clearevade 0b11101111 // mask out tool
#define clearprotect 0b11011111 // mask out tool
// mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

// miscelaneous IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
//
// cast reference dereference initialize
#define at(x) (&(x))
#define cast(x, t) ((t)(x))
#define litstrlen(datastr) (sizeof(datastr) - 1)
#define in(x) (*(x))
#define invoidptr(x) (in(cast((x), void**)))
//
// deadcode
#define deadline
//
// directions
#define prevd 0
#define nextd 1
//
// nullptr
#ifndef nullptr
   #define nullptr ((void*)(0))
#endif // nullptr
#define nomsg nullptr
#define nobuffer nullptr
#define nnn NULLPTR
//
// stacks
#define charspace(x) cast(((char [(x) + 1]){[(x)] = 0}), u8*)
#define shortspace(x) cast(((u16 [(x) + 1]){[(x)] = 0}), u16*)
#define intspace(x) cast(((u32 [(x) + 1]){[(x)] = 0}), u32*)
#define longspace(x) cast(((u64 [(x) + 1]){[(x)] = 0}), u64*)
//
// stack strs
#define datastr(arr) cast(((u64 [2]){[0] = tolen7(litstrlen((arr)), chardata), [1] = cast((arr), u64)}), str*)
#define wdatastr(arr) cast(((u64 [2]){[0] = tolen7(litstrlen((arr)), shortdata), [1] = cast((arr), u64)}), str*)
#define charstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), charstack), [1] = cast((arr), u64)}), str*)
#define shortstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), shortstack), [1] = cast((arr), u64)}), str*)
#define intstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), intstack), [1] = cast((arr), u64)}), str*)
#define longstr(len, arr) cast(((u64 [2]){[0] = tolen7((len), longstack), [1] = cast((arr), u64)}), str*)
#define buffer(len) charstr((len), charspace((len))) // len must be a constant
#define shortbuffer(len) shortstr((len), shortspace((len))) // len must be a constant
#define intbuffer(len) intstr((len), intspace((len))) // len must be a constant
#define longbuffer(len) longstr((len), longspace((len))) // len must be a constant
//
// heap strs
#define hcharstr(len, arr) bufstr(tolen7((len), charheap), (arr))
#define hshortstr(len, arr) bufstr(tolen7((len), shortheap), (arr))
#define hintstr(len, arr) bufstr(tolen7((len), intheap), (arr))
#define hlongstr(len, arr) bufstr(tolen7((len), longheap), (arr))
#define hbuffer(len) bufstr(tolen7((len), charheap), nullptr)
#define hshortbuffer(len) bufstr(tolen7((len), shortheap), nullptr)
#define hintbuffer(len) bufstr(tolen7((len), intheap), nullptr)
#define hlongbuffer(len) bufstr(tolen7((len), longheap), nullptr)
//
// indexing strs
#define elem8(s, i) in((cast(s->array, u8*)) + (i))
#define elem16(s, i) in((cast(s->array, u16*)) + (i))
#define elem32(s, i) in((cast(s->array, u32*)) + (i))
#define elem64(s, i) in((cast(s->array, u64*)) + (i))
//
// static inline 
#ifdef NOUSESTATICINLINE
   #define stinl 
#else
   #define stinl static inline __attribute__((always_inline))
#endif // NOUSESTATICINLINE
//
#ifdef NOUSEREGS
   #define rg
#else
   #define rg register
#endif // NOUSESTATICINLINE
// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// output OUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUT
#if defined(USEMYIO) && defined(PRINTDEBUG)
   #include "printf.h"
   #define explain(format, ...) console_explain(#format, __VA_ARGS__); fflush(stdout)
   #define say(cstr) console_explain(%s, #cstr)
   #define sayc(c) console_sayc(##c, stdout)
   #define sayline(cstr) console_say(#cstr)
   #define saycstr(cstr) console_say((cstr))
   #define saynum(num) console_explain("%ld\n", cast((num), i64))
   #define sayhex(hex) console_explain("%lx\n", cast((hex), u64))
#elif PRINTDEBUG
   #include <stdio.h>
   #define explain(format, ...) printf(#format, __VA_ARGS__); fflush(stdout)
   #define say(cstr) explain(%s, #cstr) 
   #define sayc(c) putc(##c, stdout)
   #define sayline(cstr) puts(#cstr)
   #define saycstr(cstr) puts((cstr))
   #define saynum(num) explain(%ld\n, cast((num), i64))
   #define sayhex(hex) explain(%lx\n, cast((hex), u64))
#else
   #define explain(format, ...)
   #define say(cstr)
   #define sayc(c)
   #define sayline(cstr)
   #define saycstr(cstr)
   #define saynum(num)
   #define sayhex(hex)
#endif // USEMYIO
// OUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUTOUT

// str attributes sssssssssssssssssssssssssssssssssssssss
#define chardata (kinddata | t1)
#define shortdata (kinddata | t2)
#define intdata (kinddata | t4)
#define longdata (kinddata | t8)
//
#define charstack (kindstack | t1)
#define shortstack (kindstack | t2)
#define intstack (kindstack | t4)
#define longstack (kindstack | t8)
//
#define charheap (kindheap | t1)
#define shortheap (kindheap | t2)
#define intheap (kindheap | t4)
#define longheap (kindheap | t8)
//
#define evade(t) (memevade | (t))
#define protect(t) (memprotect | (t))
#define evadeprotect(t) (memprotect | memevade | (t))
// ssssssssssssssssssssssssssssssssssssssssssssssssssssss

// types /////////// 
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define maxhold_u8 (0xff)
#define maxhold_u16 (0xffff)
#define maxhold_u32 (0xffffffff)
#define maxhold_u64 (0xffffffffffffffff)
#define maxhold_i8 (0x7f)
#define maxhold_i16 (0x7fff)
#define maxhold_i32 (0x7fffffff)
#define maxhold_i64 (0x7fffffffffffffff)
////////////////////

// DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
#ifdef PRINTDEBUG
   u64 times_freed = 0;
   u64 times_alloced = 0;
   #define allocPlusPlus(x) ((times_alloced += ((x) != 0)) & 0x0000000000000000) +
   #define reallocPlusPlus(ptr) ((times_alloced += (!(ptr))) & 0x0000000000000000) +
   #define freedPlusPlus(x) times_freed += ((x) != 0);
   #define RETURN explain(==================\nALLOC COUNT %lu\nFREED COUNT %lu\n==================\n\n, times_alloced, times_freed); return 0
#elif
   #define allocPlusPlus(x) 
   #define reallocPlusPlus(ptr) 
   #define freedPlusPlus(x) 
   #define RETURN return 0
#endif
// DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG

#endif // ___TAD
