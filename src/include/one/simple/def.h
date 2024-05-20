#ifndef def_h
#define def_h

#include "types.h"


/* 
     These are to make 
     
          incrementing,
          indexing,
          inlining,
     
     easier to code
                          */

#define off64(ptr, offset) ( u64p((ptr)) + (offset) )
#define off32(ptr, offset) ( u32p((ptr)) + (offset) )
#define off16(ptr, offset) ( u16p((ptr)) + (offset) )
#define  off8(ptr, offset) (  u8p((ptr)) + (offset) )

#define in64off(ptr, offset) ( *( off64((ptr), (offset)) ) )
#define in32off(ptr, offset) ( *( off32((ptr), (offset)) ) )
#define in16off(ptr, offset) ( *( off16((ptr), (offset)) ) )
#define  in8off(ptr, offset) ( *(  off8((ptr), (offset)) ) )

#define in64(ptr) ( *( off64((ptr), 0) ) )
#define in32(ptr) ( *( off32((ptr), 0) ) )
#define in16(ptr) ( *( off16((ptr), 0) ) )
#define  in8(ptr) ( *(  off8((ptr), 0) ) )

/* 
     These are to make 
     
          condition checking,
     
     easier to code
                                */

#endif // def_h
