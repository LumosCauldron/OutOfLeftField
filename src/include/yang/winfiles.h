#ifndef STREAM_H
#define STREAM_H

// windows
#include "native/include/winnt.h"
#include "native/include/winternl.h"
#include "native/include/ddk/ntifs.h"


#include "../base/water.h"
#include "../base/base_lang.h"
#include "../units.h"

typedef i64 STREAM;
typedef int MODE;

#define executemode 1
#define writemode 2
#define readmode 4 
#define readwrite 6
#define specialmode 7
#define createmode 8

#define invalid_stream -1

// _______________________________________________________
// filepath_check
// 
// globals
   #define max_path_supported 260 // avoids multiple syscalls
// _______________________________________________________
stinl u8 filepath_check(str* filename)
{  
   // check filename string is good
   nope_ptr_ret0(filename);
   
   // check filename is not empty
   nope_expr_ret0(!isemptystr(filename));
   
   // check filename is not too long
   nope_expr_ret0(getlen(filename) <= max_path_supported);
   
   return 1;
}

// create/open files by fullpaths
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
       case specialmode : ; generic_access = GENERIC_ALL;
                            how_to_open = OPEN_EXISTING;
                    break;
       default : ;  nope_msg_retx(0, "unknown mode specified", invalid_stream);
                    break;
    }

    STREAM stream = CreateFileA(filename->array, 
                                generic_access, 
                                0, NULL, 
                                how_to_open, 
                                FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH | FILE_ATTRIBUTE_HIDDEN, 
                                NULL);

    retWindowsErrorX(stream != invalid_stream, invalid_stream);

        // debugging
        #ifdef DEBUG
             if (access != createmode)
             {
                say("%s : %s\n", "open called", getarray(filename));
             }
             else
             {
                say("%s : %s\n", "create called", getarray(filename));
             }
          
             say("%s : %d\n", "fd given", stream);
        #endif

    return stream;
}

void file_close(STREAM stream)
{
   nope_expr(stream != invalid_stream);
   u8 handle_closed = CloseHandle(stream);
   retWindowsError(handle_closed);
}

u8 file_exists(str* filename)
{
   nope_expr_ret0(filepath_check(filename));
   u32 attributes = GetFileAttributes(filename->array);
   return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
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
   nope_expr_retx(filepath_check(filename), invalid_stream);
   
   u8 failflag = DeleteFileA(getarray(filename));
   retWindowsError0(failflag != 0);
   
   say("%s -/-> %s\n", "DeleteFileA called", getarray(filename));
   
   return 1;
}




#endif // STREAM_H
