#ifndef UNITS_H
#define UNITS_H

#include "water.h"

struct bytes
{
   u64 len7;
   void* array;
};

typedef struct bytes str;

// protos
stinl str* bufstr(u64, void*);
stinl str* cpystr(str* src, str* dest);
stinl u8 get7(str*);
stinl u64 getlen(str* b);
stinl str* initstr(str*, u8);
stinl u8 typeofstr(str*);

// KEY FUNCTIONS BEGIN /////////////////////////////////////////////////////////////////////
stinl u8* memto(void* destvoid, void* srcvoid, u64 len)
{
   #define dest (cast(destvoid, u8*))
   #define src (cast(srcvoid, u8*))
   assertret0(dest, "nnn dest given to memto");
   if (!len || !src || dest == src)
   {
	   return dest;
	}

   register u64 blocks   = len >> 3; // Hold number of 8-byte 'blocks' to copy
   register u64 leftover = len & 0b00000111;	// Hold number of 'leftover' bytes
   register u64 i;
   if (dest > src && dest < (src + len)) // If overlap
   {
	   u64* destptr = (u64*)(dest);
	   u64* srcptr  = (u64*)(src);
	   for (i = 1; i <= leftover; ++i)
	   {
		   *(dest + len - i) = *(src + len - i);
	   }
	   for (i = blocks - 1; i >= 0; --i)
		{   
		   *(destptr + i) = *(srcptr + i);
      }
   }
   else // regular execution
   {
	   u64* destptr = (u64*)(dest);
	   u64* srcptr  = (u64*)(src);
	   for (i = 0; i < blocks; ++i)
	   {
		   *(destptr + i) = *(srcptr + i);
	   }
	   for (i = leftover; i > 0; --i)
		{
		   *(dest + len - i) = *(src + len - i);
      }
   }
   return dest;
   #undef dest
   #undef src
}
stinl u64 tilnul(void* bytes)
{
   assertret0(bytes, nnn bytes given to tilnul);
   
   rg u64 count = 0;
   rg u8* original = cast(bytes, u8*);
   
   // While no nulterminator found
   while (in(cast(bytes, u8*)))
   {
      bytes = original + (++count);
   }
   return count;
}
void zeroarray(u8* ptr, u64 len)	// Zeros out an array
{
   assertret(ptr, nnn bytes given to tilnul);
   
   register u64 blocks    = len / sizeof(u64);
   register u64 leftover  = len % sizeof(u64);
   register u64 i;
   u64* blockptr = (u64*)(ptr);

   for (i = 0; i < blocks; ++i)
   {
	   *(blockptr + i) = 0;	
   }
   for (i = 1; i <= leftover; ++i)	
	{
	  *(ptr + len - i) = 0; 
   }
}
// KEY FUNCTIONS END ///////////////////////////////////////////////////////////////////////

// sets 
stinl u64 tolen7(u64 len, u8 b7)
{
   return (0x00ffffffffffffff & len) | (cast(b7, u64) << 56);
}

stinl void set7(str* b, u8 b7)
{
   assertret(b, nnn given to set7);
   b->len7 = tolen7(b->len7, b7);
}

stinl void setevade(str* b, u8 e)
{
   assertret(b, nnn given to setevade);
   set7(b, (get7(b) & clearevade) | memevade & e);
}

stinl void setkind(str* b, u8 k)
{
   assertret(b, nnn given to setkind);
   set7(b, (get7(b) & clearkind) | memkind & k);
}

stinl void setprotect(str* b, u8 p)
{
   assertret(b, nnn given to setprotect);
   set7(b, (get7(b) & clearprotect) | memprotect & p);
}

stinl void settype(str* b, u8 t)
{
   assertret(b, nnn given to settype);
   set7(b, (get7(b) & cleartype) | memtype & t);
}

stinl void setlen(str* b, u64 len)
{
   assertret(b, nnn given to setlen);
   b->len7 = (b->len7 & 0xff00000000000000) | (0x00ffffffffffffff & len);
}

stinl void incrlen(str* b, u64 amt)
{
   assertret(b, nnn given to incrlen);
   b->len7 = (b->len7 & 0xff00000000000000) | (0x00ffffffffffffff & (b->len7 + amt));
}

stinl void decrlen(str* b, u64 amt)
{
   assertret(b, nnn given to decrlen);
   b->len7 = (b->len7 & 0xff00000000000000) | (0x00ffffffffffffff & (b->len7 - amt));
}

stinl void setlen7(str* b, u64 len7)
{
   assertret(b, nnn given to setlen7);
   b->len7 = len7;
}

stinl void setarray(str* b, void* array)
{
   assertret(b, nnn given to setarray);
   b->array = array;
}

stinl void setarraylen7(str* b, void* array, u64 len, u8 b7)
{
   assertret(b, nnn given to setarraylen7);
   setarray(b, array);
   setlen7(b, tolen7(len, b7));
}

stinl void setstr(str* src, str* dest)
{
   assertret(src, nnn src given to setstr);
   assertret(dest, nnn dest given to setstr);
   dest->array = src->array;
   dest->len7 = src->len7;
}


// gets
stinl u8 get7(str* b)
{
   assertret0(b, nnn given to get7);
   return (b->len7 & 0xff00000000000000) >> 56;
}

stinl u8 getevade(str* b)
{
   assertret0(b, nnn given to getevade);
   return get7(b) & memevade;
}

stinl u8 getkind(str* b)
{
   assertret0(b, nnn given to getkind);
   return get7(b) & memkind;
}

stinl u8 getprotect(str* b)
{
   assertret0(b, nnn given to getprotect);
   return get7(b) & memprotect;
}

stinl u8 gettype(str* b)
{
   assertret0(b, nnn given to gettype);
   return get7(b) & memtype;
}

stinl u64 lenoflen7(u64 len7)
{
   return 0x00ffffffffffffff & len7;
}

stinl u64 index(str* b, u64 i)
{
   assertret0(b, nnn given to index);
   assertret0(i < getlen(b), index i is larger than str length); // return 0
   
   switch (typeofstr(b))
   {
      case t1 : return elem8(b, i);
      case t8 : return elem64(b, i);
      case t2 : return elem16(b, i);
      case t4 : return elem32(b, i);
      default : return elem8(b, i);
   }
}

stinl u64 getlen(str* b)
{
   assertret0(b, nnn given to getlen);
   return 0x00ffffffffffffff & b->len7;
}

stinl u64 getlenbytes(str* b)
{
   assertret0(b, nnn given to getlenbytes);
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
   assertret0(b, nnn given to getlen7);
   return b->len7;
}

stinl u8* getarray(str* b)
{
   assertret0(b, nnn given to getarray);
   return cast(b->array, u8*);
}

// get attributes
stinl u8 typeofstr(str* b)
{
   assertret0(b, nnn given to typeofstr);
   return memtype & get7(b);
}
stinl u8 kindofstr(str* b)
{
   assertret0(b, nnn given to kindofstr);
   return memkind & get7(b);
}
stinl u8 evadingstr(str* b)
{
   assertret0(b, nnn given to evadingstr);
   return memevade & get7(b);
}
stinl u8 protectedstr(str* b)
{
   assertret0(b, nnn given to protectedstr);
   return memprotect & get7(b);
}
stinl u8 isemptystr(str* b) // length of 1 pointing to a nul-terminator
{
   assertret0(b, nnn given to isemptystr);
   return ((getlen(b) == 1) && (index(b, 0) == 0));
}

// ops

// all appends need to be freed
stinl str* appendstr(str* mainstr, str* toadd)
{
   assertretx(mainstr || toadd, nnn mainstr and nnn toadd given to appendstr, mainstr);
   if (!mainstr && !toadd)
   {
      return nullptr;
   }
   if (!mainstr)
   {
      str* newstr = hbuffer(getlenbytes(toadd));
      return cpystr(toadd, newstr);
   }
   if (!toadd)
   {
      return mainstr;
   }
   
   // calc lengths
   register u64 mainlen = getlenbytes(mainstr);
   register u64 toaddlen = getlenbytes(toadd);
   register u64 bothlen = mainlen + toaddlen;
   u8* dest;
   
   // change kind if we need to morph 
   // from a data or stack into a heapstr
   if (getkind(mainstr) != kindheap)
   {
      dest = cast(give(bothlen + 1), u8*);
      memto(dest, getarray(mainstr), mainlen);
      memto(dest + mainlen, getarray(toadd), toaddlen);
      mainstr = hcharstr(bothlen, dest);
   }
   else
   {
      dest = cast(regive(getarray(mainstr), bothlen + 1), u8*);
      memto(dest + mainlen, getarray(toadd), toaddlen);
   }

   // nul terminate
   in(dest + bothlen) = 0;
   
   // set and return
   setarray(mainstr, dest);
   setlen(mainstr, bothlen);
   
   return mainstr;
}

// all bufstrs need to be freed
stinl str* bufstr(u64 len7, void* bytes)
{
   str* newstr = give(sizeof(str));
   if (newstr)
   {
      register u64 len = lenoflen7(len7);
      newstr->len7 = len7;
      if (!bytes)
      {
         switch (typeofstr(newstr))
         {
            case t1 : newstr->array = give(len + 1);
                      in(cast(newstr->array, u8*) + len) = 0; 
                      break;
            case t8 : newstr->array = give((len + 1) << 3); // mult by 8 bytes
                      in(cast(newstr->array, u64*) + len) = 0; 
                      break;
            case t2 : newstr->array = give((len + 1) << 1); // mult by 2 bytes
                      in(cast(newstr->array, u16*) + len) = 0; 
                      break;
            case t4 : newstr->array = give((len + 1) << 2); // mult by 4 bytes
                      in(cast(newstr->array, u32*) + len) = 0; 
                      break;
            default : newstr->array = give(len + 1);
                      in(cast(newstr->array, u8*) + len) = 0; 
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

stinl str* cpystr(str* src, str* dest)
{
   assertret0(src, nnn src given to cpystr);
   assertret0(dest, nnn dest given to cpystr);
   assertret0(getlenbytes(dest) >= getlenbytes(src), dest must have enough memory to store data from src);
   
   rg u64 srclen = getlenbytes(src);
   rg u64 srcblks = srclen >> 3; // div 8
   rg u64 srcrem = srclen & 0b00000111; // mod 8
   rg i64 i; // !!! KEEP SIGNED !!!
   
   u64* srcptr64 = cast(getarray(src), u64*);
   u64* destptr64 = cast(getarray(dest), u64*);
   u8* srcptr8 = getarray(src);
   u8* destptr8 = getarray(dest);
   
   // If dest overlaps src
   if (destptr8 > srcptr8 && destptr8 < (srcptr8 + srclen))
   {
      // Go right to left
      for (i = 0; i < srcrem; ++i)
      {
         in(destptr8 + (srclen - i)) = in(srcptr8 + (srclen - i));
      }
      for (i = srcblks - 1; i >= 0; --i)
      {
         in(destptr64 + i) = in(srcptr64 + i);
      }
   }
   else
   {
      // Go left to right
      for (i = 0; i < srcblks; ++i)
      {
         in(destptr64 + i) = in(srcptr64 + i);
      }
      for (i = srcrem; i > 0; --i)
      {
         in(destptr8 + (srclen - i)) = in(srcptr8 + (srclen - i));
      }
   }
   
   return dest;
}

stinl u8 eqstr(str* src, str* dest)
{
   assertret0(src, nnn src given to eqstr);
   assertret0(dest, nnn dest given to eqstr);
   
   rg u64 destlen = getlenbytes(dest);
   rg u64 srclen = getlenbytes(src);
   
   if (destlen ^ srclen)
   {
      return 0;
   }
   
   rg u64 srcblks = srclen >> 3; // div 8
   rg u64 srcrem = srclen & 0b00000111; // mod 8
   rg i64 i; // !!! KEEP SIGNED !!!
   
   u64* srcptr64 = cast(getarray(src), u64*);
   u64* destptr64 = cast(getarray(dest), u64*);
   u8* srcptr8 = getarray(src);
   u8* destptr8 = getarray(dest);
   
   // If dest overlaps src
   for (i = 0; i < srcblks; ++i)
   {
      if (in(destptr64 + i) ^ in(srcptr64 + i))
      {
         return 0;
      }
   }
   for (i = srcrem; i > 0; --i)
   {
      if (in(destptr8 + (srclen - i)) ^ in(srcptr8 + (srclen - i)))
      {
         return 0;
      }
   }
   return 1;
}

stinl void freestr(void* ptr)
{
   if (ptr)
   {
      if (invoidptr(ptr))
      {
         str* b = cast(invoidptr(ptr), str*);
         
         register u8* array = getarray(b);
         register u8 evade = evadingstr(b);
         register u8 kind = kindofstr(b);
         if (array)
         {
            // zeroize
            if (evade && (kind == kindstack || kind == kindheap))
            {
               initstr(b, 0);
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

stinl str* initstr(str* b, u8 c)
{
   assertret0(b, nnn b given to initstr);
   
   rg u64 c64 = 0;
   if (c)
   {
      c64 = cast(c, u64) | 
            (cast(c, u64) << 8) | 
            (cast(c, u64) << 16) |
            (cast(c, u64) << 24) | 
            (cast(c, u64) << 32) | 
            (cast(c, u64) << 40) | 
            (cast(c, u64) << 48) | 
            (cast(c, u64) << 56);
   }
               
   rg u64 len = getlenbytes(b);
   rg u64 blks = len >> 3; // div 8
   rg u64 rem = len & 0b00000111; // mod 8
   rg i64 i; // !!! KEEP SIGNED !!!
   
   volatile rg u64* ptr64 = cast(getarray(b), u64*);
   volatile rg u8* ptr8 = cast(getarray(b), u8*);
   
   for (i = 0; i < blks; ++i)
   {
      in(ptr64 + i) = c64; // Init. by 8 bytes
   }
   for (i = rem; i > 0; --i)
   {
      in(ptr8 + (len - i)) = c; // Init. per byte
   }
   
   return b;
}

stinl str* xorstr(str* src, str* dest)
{
   assertret0(src, nnn src given to xorstr);
   assertret0(dest, nnn dest given to xorstr);
   
   rg u64 srclen = getlenbytes(src);
   rg u64 srcblks = srclen >> 3; // div 8
   rg u64 srcrem = srclen & 0b00000111; // mod 8
   rg i64 i; // !!! KEEP SIGNED !!!
   
   u64* srcptr64 = cast(getarray(src), u64*);
   u64* destptr64 = cast(getarray(dest), u64*);
   u8* srcptr8 = getarray(src);
   u8* destptr8 = getarray(dest);
   
   // If dest overlaps src
   if (getarray(dest) > getarray(src) && getarray(dest) < (getarray(src) + srclen))
   {
      // Go right to left
      for (i = 0; i < srcrem; ++i)
      {
         in(destptr8 + (srclen - i)) ^= in(srcptr8 + (srclen - i));
      }
      for (i = srcblks - 1; i >= 0; --i)
      {
         in(destptr64 + i) ^= in(srcptr64 + i);
      }
   }
   else
   {
      // Go left to right
      for (i = 0; i < srcblks; ++i)
      {
         in(destptr64 + i) ^= in(srcptr64 + i);
      }
      for (i = srcrem; i > 0; --i)
      {
         in(destptr8 + (srclen - i)) ^= in(srcptr8 + (srclen - i));
      }
   }
   
   return dest;
}

#ifdef PRINTDEBUG
void printstr(str* s)
{
   if (!s)
   {
      return;
   }
   
   sayline(_________________);
   switch(kindofstr(s))
   {
      case kinddata : sayline(KIND -> data);
         break;
      case kindstack: sayline(KIND -> stack);
         break;
      case kindheap : sayline(KIND -> heap);
         break;
      default : sayline(KIND -> unknown);
   }
   
   u8 stringtype = typeofstr(s);
   switch(typeofstr(s))
   {
      case t1 : sayline(TYPE -> 8 CHAR);
         break;
      case t2 : sayline(TYPE -> 16 SHORT);
         break;
      case t4 : sayline(TYPE -> 32 INT);
         break;
      case t8 : sayline(TYPE -> 64 LONG/PTR);
         break;
      default : sayline(TYPE -> unknown);
   }
   
   u64 len = getlen(s);
   say("LEN  =-> ");
   saynum(len);
   
   say("ARR* =-> ");
   u64 i;
   for (i = 0; i < len; ++i)
   {
      switch(stringtype)
      {
         case t1 : explain(%c, cast(index(s, i), char));
            break;
         case t8 : explain(%lu , cast(index(s, i), i64));
            break;
         case t4 : explain(%lu , cast(index(s, i), i32));
            break;
         case t2 : explain(%lu , cast(index(s, i), i16));
            break;
         default : 
            break;
      }
   }
   sayline();
   
   say("SEE  =-> ");
   for (; len > 0; --len)
   {
      say(*);
   }
   sayline();
   
   if (evadingstr(s))
   {
      sayline(evading);
   }
   if (protectedstr(s))
   {
      sayline(protected);
   }
   sayline(_________________);
}
#else
void printstr(str* s) 
{
   return;
}
#endif

#endif // UNITS_H
