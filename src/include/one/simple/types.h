#ifndef u8_h
#define u8_h

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

#endif // u8_h
