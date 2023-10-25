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

#define INVALID_HANDLE_VALUE -1

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ACCESS MODES
// 
// FILE_GENERIC_EXECUTE 	
// 
//    FILE_EXECUTE
//    FILE_READ_ATTRIBUTES
//    STANDARD_RIGHTS_EXECUTE
//    SYNCHRONIZE
// 
// FILE_GENERIC_READ 	
// 
//    FILE_READ_ATTRIBUTES
//    FILE_READ_DATA
//    FILE_READ_EA
//    STANDARD_RIGHTS_READ
//    SYNCHRONIZE
// 
// FILE_GENERIC_WRITE 	
// 
//    FILE_APPEND_DATA
//    FILE_WRITE_ATTRIBUTES
//    FILE_WRITE_DATA
//    FILE_WRITE_EA
//    STANDARD_RIGHTS_WRITE
//    SYNCHRONIZE

#define readmode \
(FILE_TRAVERSE | FILE_READ_ATTRIBUTES | FILE_READ_DATA | FILE_READ_EA | STANDARD_RIGHTS_READ | SYNCHRONIZE)
#define writemode \
(FILE_TRAVERSE | FILE_APPEND_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_DATA | FILE_WRITE_EA | STANDARD_RIGHTS_WRITE | SYNCHRONIZE)
#define executemode \
(FILE_TRAVERSE | FILE_EXECUTE | FILE_READ_ATTRIBUTES | STANDARD_RIGHTS_EXECUTE | SYNCHRONIZE)
#define specialmode \
(FILE_TRAVERSE | DELETE | WRITE_DAC | WRITE_OWNER)
#define createmode \
(readmode | writemode | executemode | specialmode)


stinl void file_close(STREAM stream)
{
   assertion(stream != cast(INVALID_HANDLE_VALUE, i64), invalid handle value -1 given to file_close);
   assertion(NtClose(cast(stream, void*)), stream failed to close in file_close);
}

/*
stinl u8 file_destroy(str* filename)
{
   // make space
   char fullname[getlen(filename) + 5];
   u8 failflag = 0;
   
   // add the prefix "\\?\" to support 32767 bytes
   //file_prefix(fullname, filename); 
   
   // delete the file
   sayline({}{}{}{}{}{}{}{}{});
   explain(%s : %s\n, "destroy", getarray(filename));
   // start here.....
      failflag = DeleteFileA(getarray(filename));
   explain(%s : %ld\n, "error", GetLastError());
   sayline({}{}{}{}{}{}{}{}{});
      assertret(failflag, failed to destroy a file);
   return 1; // returns 0 if assertion fails
}*/

// create/open files by fullpaths
stinl STREAM file_open(str* filename, MODE access) 
{  
   const u32 prefixlen = 4;                                // actual length without '\0' is namelen - 1
   register u64 namelen = getlen(filename) + prefixlen + 1; // dumb naming on my part my bad
   assertretx(namelen <= maxhold_i16, filename/path is too large, -1);
   
   // hold prefix and assign in one shot like a boss
   register u32 prefix = 0x5c3f3f5c; 
   u16 fullname[namelen];
   
   // "\??\" assigned as prefix in one shot
   in(cast(fullname, u32*)) = prefix;
   
   // fullname nul-terminated
   fullname[namelen - 1] = 0x0000; 

   // assign fullname u16 array but continue to use 'fullname' to manipulate 
   dumbStrW unicodeFileName = 
   { 
      namelen - 1, // actual length
      namelen, // max len of usable memory
      cast(fullname, u16*) // wchar pointer to full name
   };
   
   // if char array, translate to wide char array (skip over prefix)
   if (typeofstr(filename) == t1)
   {
      lang_mbtow(fullname + prefixlen, getarray(filename), getlenbytes(filename));
   }
   else
   {
      memto(fullname + prefixlen, getarray(filename), getlenbytes(filename));
   }
   // set up objects for system nt/zw call
   OBJECT_ATTRIBUTES objAttr; 
   
   // #define InitializeObjectAttributes(p,n,a,r,s) DEFINED MACRO
   objAttr.Length = sizeof(OBJECT_ATTRIBUTES);
   objAttr.RootDirectory = NULL; // r (this right here makes it so we must use full paths)
   objAttr.Attributes = OBJ_CASE_INSENSITIVE | OBJ_EXCLUSIVE | OBJ_INHERIT; // a
   objAttr.ObjectName = cast(&unicodeFileName, PUNICODE_STRING); // n
   objAttr.SecurityDescriptor = NULL; // s
   objAttr.SecurityQualityOfService = NULL;

   IO_STATUS_BLOCK ioStatusBlk;
   HANDLE handle;
   register MODE openmode;
   register NTSTATUS ntStatus;
    
   sayline([][][][][][][][][]);
   if (access == readmode || access == executemode || access == specialmode)
   {
      openmode = FILE_OPEN;
      explain(%s : %s\n, "open", getarray(filename));
   }
   else
   {  // will replace if file exists already
      openmode = FILE_SUPERSEDE;
      explain(%s : %s\n, "create", getarray(filename));
   }

   // create/open the file with desired parameters
   ntStatus = NtCreateFile(
                             &handle, // out
                             access, // out
                             &objAttr,
                             &ioStatusBlk,
                             NULL,
                             FILE_ATTRIBUTE_SYSTEM |                 // attr
                             FILE_ATTRIBUTE_NOT_CONTENT_INDEXED |    // attr
                             FILE_ATTRIBUTE_HIDDEN,                  // attr
                             FILE_SHARE_NOTOUCHY,                    // share
                             openmode,
                             FILE_NO_INTERMEDIATE_BUFFERING |        // options
                             FILE_NON_DIRECTORY_FILE |               // options
                             FILE_WRITE_THROUGH |                    // options
                             FILE_RANDOM_ACCESS |                    // options
                             FILE_SYNCHRONOUS_IO_NONALERT,           // options
                             NULL,
                             0
                          );
                         
   explain(%s : %p\n, "handle", handle);
   explain(%s : %ld\n, "error", ntStatus);
   sayline([][][][][][][][][]);
   
   if (NT_SUCCESS(ntStatus))
   {
      return cast(handle, i64);
   }
   else
   {
      return -1;
   }
}

/*
stinl u64 file_read(STREAM stream, u8* buffer, u64 toread)
{
   register u64 amount = 0;
   while (amount += read(stream, buffer + amount, toread - amount) < toread);
   return amount;
}
stinl u64 file_readstr(STREAM stream, str* space)
{
   return file_read(stream, getarray(space), getlen(space));
}

stinl u64 file_write(STREAM stream, u8* buffer, u64 towrite)
{
   register u64 amount = 0;
   while (amount += write(stream, buffer + amount, towrite - amount) < towrite);
   return amount;
}
stinl u64 file_writestr(STREAM stream, str* data)
{
   return file_write(stream, getarray(space), getlen(space));
}*/

#endif // STREAM_H
