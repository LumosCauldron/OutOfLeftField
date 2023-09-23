#ifndef STREAM_H
#define STREAM_H

#include "base/water.h"
#include "units.h"

#include <windows.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ACCESS MODES
/*
FILE_GENERIC_EXECUTE 	

   FILE_EXECUTE
   FILE_READ_ATTRIBUTES
   STANDARD_RIGHTS_EXECUTE
   SYNCHRONIZE

FILE_GENERIC_READ 	

   FILE_READ_ATTRIBUTES
   FILE_READ_DATA
   FILE_READ_EA
   STANDARD_RIGHTS_READ
   SYNCHRONIZE

FILE_GENERIC_WRITE 	

   FILE_APPEND_DATA
   FILE_WRITE_ATTRIBUTES
   FILE_WRITE_DATA
   FILE_WRITE_EA
   STANDARD_RIGHTS_WRITE
   SYNCHRONIZE
   
   
   
ALL ACCESS RIGHTS

   FILE_ADD_FILE
   VALUE 2
   DESCRIPTION
      For a directory, the right to create a file in the directory.

   FILE_ADD_SUBDIRECTORY
   VALUE 4
   DESCRIPTION
      For a directory, the right to create a subdirectory.

   FILE_ALL_ACCESS
   DESCRIPTION
      All possible access rights for a file.

   FILE_APPEND_DATA
   VALUE 4
   DESCRIPTION
      For a file object, the right to append data to the file. (For local files, write 
      operations will not overwrite existing data if this flag is specified without 
      FILE_WRITE_DATA.) For a directory object, the right to create a subdirectory 
      (FILE_ADD_SUBDIRECTORY).

   FILE_CREATE_PIPE_INSTANCE
   VALUE 4
   DESCRIPTION
      For a named pipe, the right to create a pipe.

   FILE_DELETE_CHILD
   VALUE 64 (0x40)
   DESCRIPTION
      For a directory, the right to delete a directory and all the files it contains, 
      including read-only files.

   FILE_EXECUTE
   VALUE 32 (0x20)
   DESCRIPTION
      For a native code file, the right to execute the file. This access right given to 
      scripts may cause the script to be executable, depending on the script 
      interpreter.

   FILE_LIST_DIRECTORY
   VALUE 1
   DESCRIPTION
      For a directory, the right to list the contents of the directory.

   FILE_READ_ATTRIBUTES
   VALUE 128 (0x80)
   DESCRIPTION
      The right to read file attributes.

   FILE_READ_DATA
   VALUE 1
   DESCRIPTION
      For a file object, the right to read the corresponding file data. For a directory 
      object, the right to read the corresponding directory data.

   FILE_READ_EA
   VALUE 8
   DESCRIPTION
      The right to read extended file attributes.

   FILE_TRAVERSE
   VALUE 32 (0x20)
   DESCRIPTION
      For a directory, the right to traverse the directory. By default, users are 
      assigned the BYPASS_TRAVERSE_CHECKING privilege, which ignores the FILE_TRAVERSE 
      access right. See the remarks in File Security and Access Rights for more information.

   FILE_WRITE_ATTRIBUTES
   VALUE 256 (0x100)
   DESCRIPTION
      The right to write file attributes.

   FILE_WRITE_DATA
   VALUE 2
   DESCRIPTION
      For a file object, the right to write data to the file. For a directory object, 
      the right to create a file in the directory (FILE_ADD_FILE).

   FILE_WRITE_EA
   VALUE 16 (0x10)
   DESCRIPTION
      The right to write extended file attributes.

   STANDARD_RIGHTS_READ
   DESCRIPTION
      Includes READ_CONTROL, which is the right to read the information in the file 
      or directory object's security descriptor. This does not include the information 
      in the SACL.

   STANDARD_RIGHTS_WRITE

   Same as STANDARD_RIGHTS_READ.
	
*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE/OPEN MODES

#define WIN_CREATE_ALWAYS 2
/*
Creates a new file, always.
If the specified file exists and is writable, the function overwrites the file, the function succeeds, and last-error code is set to ERROR_ALREADY_EXISTS.
If the specified file does not exist and is a valid path, a new file is created, the function succeeds, and the last-error code is set to zero.
*/


#define WIN_CREATE_NEW 1
/*
Creates a new file, only if it does not already exist.
If the specified file exists, the function fails and the last-error code is set to ERROR_FILE_EXISTS.
If the specified file does not exist and is a valid path to a writable location, a new file is created.
*/


#define WIN_OPEN_ALWAYS 4
/*
Opens a file, always.
If the specified file exists, the function succeeds and the last-error code is set to ERROR_ALREADY_EXISTS.
If the specified file does not exist and is a valid path to a writable location, the function creates a file and the last-error code is set to zero.
*/


#define WIN_OPEN_EXISTING 3
/*
Opens a file or device, only if it exists.
If the specified file or device does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND.
*/


#define WIN_TRUNCATE_EXISTING 5
/*
Opens a file and truncates it so that its size is zero bytes, only if it exists.
If the specified file does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND.
The calling process must open the file with the GENERIC_WRITE bit set as part of the dwDesiredAccess parameter.
*/


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FORGET SHARING 

#define WIN_FILE_SHARE_NONE 0x00000000
/*
Prevents other processes from opening a file or device 
if they request delete, read, or write access.
*/


#define WIN_FILE_SHARE_DELETE 0x00000004
/*
Enables subsequent open operations on a file or device to request delete access.
Otherwise, other processes cannot open the file or device if they request delete access.
If this flag is not specified, but the file or device has been opened for delete access, the function fails.
Note  Delete access allows both delete and rename operations.
*/


#define WIN_FILE_SHARE_READ 0x00000001
/*
Enables subsequent open operations on a file or device to request read access.
Otherwise, other processes cannot open the file or device if they request read access.
If this flag is not specified, but the file or device has been opened for read access, the function fails.
*/


#define WIN_FILE_SHARE_WRITE 0x00000002
/*
Enables subsequent open operations on a file or device to request write access.
Otherwise, other processes cannot open the file or device if they request write access.
If this flag is not specified, but the file or device has been opened for write access or has a file mapping with write access, the function fails.
*/




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ATTRIBUTE FLAGS

#define WIN_FILE_ATTRIBUTE_ARCHIVE 0x20 // no
// The file should be archived. Applications use this attribute to mark files for backup or removal.


#define WIN_FILE_ATTRIBUTE_ENCRYPTED 0x4000 // no
/*
The file or directory is encrypted. For a file, this means that all data in the file is encrypted. For a directory, this means that encryption is the default for newly created files and subdirectories. For more information, see File Encryption.
This flag has no effect if FILE_ATTRIBUTE_SYSTEM is also specified.
This flag is not supported on Home, Home Premium, Starter, or ARM editions of Windows.
*/

#define WIN_FILE_ATTRIBUTE_HIDDEN 0x2
// The file is hidden. Do not include it in an ordinary directory listing.


#define WIN_FILE_ATTRIBUTE_NORMAL 0x80
// The file does not have other attributes set. This attribute is valid only if used alone.


#define WIN_FILE_ATTRIBUTE_OFFLINE 0x1000 // no
/*
The data of a file is not immediately available. This attribute indicates that file data is physically moved to offline storage. This attribute is used by Remote Storage, the hierarchical storage management software. Applications should not arbitrarily change this attribute.
*/

#define WIN_FILE_ATTRIBUTE_READONLY 0x1
// The file is read only. Applications can read the file, but cannot write to or delete it.


#define WIN_FILE_ATTRIBUTE_SYSTEM 0x4
// The file is part of or used exclusively by an operating system.


#define WIN_FILE_ATTRIBUTE_TEMPORARY 0x100 // no
// The file is being used for temporary storage.


#define WIN_FILE_FLAG_BACKUP_SEMANTICS 0x02000000
/*
The file is being opened or created for a backup or restore operation. The system ensures that the calling process overrides file security checks when the process has SE_BACKUP_NAME and SE_RESTORE_NAME privileges. For more information, see Changing Privileges in a Token.
You must set this flag to obtain a handle to a directory. A directory handle can be passed to some functions instead of a file handle. For more information, see the Remarks section.
*/


#define WIN_FILE_FLAG_DELETE_ON_CLOSE 0x04000000 // no
/*
The file is to be deleted immediately after all of its handles are closed, which includes the specified handle and any other open or duplicated handles.
If there are existing open handles to a file, the call fails unless they were all opened with the FILE_SHARE_DELETE share mode.
Subsequent open requests for the file fail, unless the FILE_SHARE_DELETE share mode is specified.
*/


#define WIN_FILE_FLAG_NO_BUFFERING 0x20000000
/*
The file or device is being opened with no system caching for data reads and writes. This flag does not affect hard disk caching or memory mapped files.
There are strict requirements for successfully working with files opened with CreateFile using the FILE_FLAG_NO_BUFFERING flag, for details see File Buffering.
*/


#define WIN_FILE_FLAG_OPEN_NO_RECALL 0x00100000 // no
/*
The file data is requested, but it should continue to be located in remote storage. It should not be transported back to local storage. This flag is for use by remote storage systems.
*/


#define WIN_FILE_FLAG_OPEN_REPARSE_POINT 0x00200000 // no
/*
Normal reparse point processing will not occur; CreateFile will attempt to open the reparse point. When a file is opened, a file handle is returned, whether or not the filter that controls the reparse point is operational.
This flag cannot be used with the CREATE_ALWAYS flag.
If the file is not a reparse point, then this flag is ignored.
*/

#define WIN_FILE_FLAG_OVERLAPPED 0x40000000 // no
/*
The file or device is being opened or created for asynchronous I/O.
When subsequent I/O operations are completed on this handle, the event specified in the OVERLAPPED structure will be set to the signaled state.
If this flag is specified, the file can be used for simultaneous read and write operations.
If this flag is not specified, then I/O operations are serialized, even if the calls to the read and write functions specify an OVERLAPPED structure.
*/


#define WIN_FILE_FLAG_POSIX_SEMANTICS 0x01000000
/*
Access will occur according to POSIX rules. This includes allowing multiple files with names, differing only in case, for file systems that support that naming. Use care when using this option, because files created with this flag may not be accessible by applications that are written for MS-DOS or 16-bit Windows.
*/


#define WIN_FILE_FLAG_RANDOM_ACCESS 0x10000000
/*
Access is intended to be random. The system can use this as a hint to optimize file caching.
This flag has no effect if the file system does not support cached I/O and FILE_FLAG_NO_BUFFERING.
*/


#define WIN_FILE_FLAG_SESSION_AWARE 0x00800000
/*
The file or device is being opened with session awareness. If this flag is not specified, then per-session devices (such as a device using RemoteFX USB Redirection) cannot be opened by processes running in session 0. This flag has no effect for callers not in session 0. This flag is supported only on server editions of Windows.
Windows Server 2008 R2 and Windows Server 2008:  This flag is not supported before Windows Server 2012.
*/


#define WIN_FILE_FLAG_SEQUENTIAL_SCAN 0x08000000
/*
Access is intended to be sequential from beginning to end. The system can use this as a hint to optimize file caching.
This flag should not be used if read-behind (that is, reverse scans) will be used.
This flag has no effect if the file system does not support cached I/O and FILE_FLAG_NO_BUFFERING. 
*/


#define WIN_FILE_FLAG_WRITE_THROUGH 0x80000000
// Write operations will not go through any intermediate cache, they will go directly to disk. 

#define readmode \
(FILE_READ_ATTRIBUTES | FILE_READ_DATA | FILE_READ_EA |STANDARD_RIGHTS_READ | SYNCHRONIZE)
#define writemode \
(FILE_APPEND_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_DATA | FILE_WRITE_EA | STANDARD_RIGHTS_WRITE | SYNCHRONIZE)
#define executemode \
(FILE_EXECUTE | FILE_READ_ATTRIBUTES | STANDARD_RIGHTS_EXECUTE | SYNCHRONIZE)
#define specialmode \
(DELETE | WRITE_DAC | WRITE_OWNER)
#define createmode \
(readmode | writemode | executemode | specialmode)


typedef i64 STREAM;
typedef int MODE;


// protos
stinl void file_prefix(char* namebuffer, str* filename);


stinl void file_close(STREAM stream)
{
   assertion(stream != cast(INVALID_HANDLE_VALUE, i64), invalid handle value -1 given to file_close);
   assertion(CloseHandle(cast(stream, void*)), stream failed to close in file_close);
}


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
}


u8 file_exists(str* filename)
{
//..
return 1;
}


stinl STREAM file_open(str* filename, MODE access)
{  
   MODE openmode;
   char fullname[getlen(filename) + 5];
   file_prefix(fullname, filename);

   sayline([][][][][][][][][]);
   if (access == readmode || access == executemode || access == specialmode)
   {
      openmode = OPEN_EXISTING;
      explain(%s : %s\n, "open", getarray(filename));
   }
   else
   {
      openmode = CREATE_ALWAYS;
      explain(%s : %s\n, "create", getarray(filename));
   }
   
   register i64 retval = cast(CreateFileA(getarray(filename), 
                              access,
                              WIN_FILE_SHARE_NONE,
                              NULL, // unused security
                              openmode,
                              FILE_ATTRIBUTE_SYSTEM | 
                              FILE_FLAG_WRITE_THROUGH | 
                              FILE_FLAG_NO_BUFFERING | 
                              FILE_ATTRIBUTE_HIDDEN, 
                              NULL), // unused template file handle (lol what? good grief windows)
                         i64); // signed because INVALID_HANDLE_VALUE is -1
   
   // Output debugging if -D PRINTDEBUG specified
   explain(%s : %ld\n, "handle", retval);
   explain(%s : %ld\n, "error", GetLastError());
   sayline([][][][][][][][][]);
   
   // create file
   return retval;
}


stinl void file_prefix(char* namebuffer, str* filename)
{
   assertion(namebuffer, nnn namebuffer given to file_prefix);
   assertion(filename, nnn filename given to file_prefix);
   
   register u64 len = getlen(filename);
   // set prefix
   memto(namebuffer, "\\\\?\\", 4);
   // set file name
   memto(namebuffer + 4, getarray(filename), len);
   // nul terminate buffer
   *(namebuffer + len + 4) = 0;
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
