#ifndef BASE_LANG_H
#define BASE_LANG_H

#include "water.h"

struct uniStrW // !
{
    u16 len;
    u16 max_len;
    u16* array;
};
typedef struct uniStrW dumbStrW;
 
struct ansiStrA // !
{
    u16 len;
    u16 max_len;
    u16* array;
};
typedef struct ansiStrA dumbStrA;

// windows notion of ansi is multibyte
// multibyte is most likely Windows-1252 

// multibyte to utf16
void lang_mbtow(u16* unistr, u8* mbstr, u64 mblen)
{
    ar(unistr, nnn given to lang_mbtow);
    ar(mbstr, nnn given to lang_mbtow);
   
    rg u64 i;
    for (i = 0; i < mblen; ++i)
    {
        *(unistr + i) = cast(0x0000 | *(mbstr + i), u16);
    }
} //. ..



#endif // BASE_LANG_H
