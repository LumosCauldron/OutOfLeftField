#ifndef FILES_H
#define FILES_H

// notes: 
// 1. improve =-> (void) __sseek((void *)fp, (0fpos_t)0, SEEK_END); maybe for O_APPEND functionality?
// 2. improve =-> give ability to change directories to avoid maximum supported path errors

// compatibility 
#ifndef O_DIRECT
   #define O_DIRECT 0 // future outlook =-> trying to avoid compilation error (we're probable compiling for Mac OSX)
#endif

// our libs
#include "../one/water.h"
#include "../one/lang.h"
#include "../one/units.h"
#include "lamplight.h"

// their libs
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// types
typedef int MODE;

// defs
#define INVALID_STREAM -1
#define UNLINK_FAILED -1
#define MAX_FILENAME 255
#define MAX_READBLOCK 4096
#define ROOT_DIRSTR "/"
#define executemode 1
#define writemode 2
#define readmode 4 
#define readwrite 6
#define specialmode 7
#define createmode 8

// globals
u64 max_path_supported = 0;

// functions
// _______________________________________________________
// file_open
// 
// _______________________________________________________
stinl STREAM file_open(str* filename, MODE access) 
{
   // DUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODE
   if (!max_path_supported)
   {
      max_path_supported = pathconf(ROOT_DIRSTR, _PC_PATH_MAX);
   }
   
   assertretx(filename, nnn filename in file_open, INVALID_STREAM);
   assertret0(!isemptystr(filename), empty filename in file_open);
   assertretx(getlen(filename) <= max_path_supported, filepath longer than 'max_path_supported' in file_open, INVALID_STREAM)
   // must be the same as file_destroy
   // DUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODE
   
   // =-> (write straight to disk linux:"O_DIRECT", osx:"fcntl(fd, F_NOCACHE, 1)"
   // =-> large files ok
   // =-> don't update date
   // =-> symlinks don't count
   // =-> ensure data don't care about metadata
   register int genflags = O_LARGEFILE | O_NOATIME  | 
                          O_NOFOLLOW | O_DSYNC | O_DIRECT;
   
   // =-> when creating/createmode: replace already existing, write only then add the above
   register int finalflags = O_CREAT | O_WRONLY | 
                           O_TRUNC | genflags;
                          
   register int mode = S_ISVTX | S_IRWXU;
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
      default: assertretx(0, what mode is wanted to open the file, INVALID_STREAM);
   }
   
   // actual open operation
   register int fd = open(getarray(filename), finalflags, mode);
   
   // debugging
   #ifdef PRINTDEBUG
      sayline([][][][][][][][][]);
      if (access != createmode)
      {
         explain(%s : %s\n, "open", getarray(filename));
      }
      else
      {
         explain(%s : %s\n, "create", getarray(filename));
      }
      explain(%s : %u\n, "fd", fd);
      explain(%s : %ld\n, "error", errno);
      sayline([][][][][][][][][]);
   #endif
   
   // =-> if bad file descriptor 
   asserterrnoretx(fd != INVALID_STREAM, open error: , INVALID_STREAM);
   
   #ifdef IZANAMI
      if (fd != INVALID_STREAM)  // same as using O_DIRECT but for OSX
      {
         fcntl(fd, F_NOCACHE, 1);
      }
   #endif
   
   return fd;
}
// _______________________________________________________
// file_close
// 
// _______________________________________________________
stinl void file_close(STREAM stream)
{
   assertret(stream >= 0, stream is less than zero in file_close... not closing this);
   
   if (stream >= 0)
   {
      close(stream);
   }
}
// _______________________________________________________
// file_exists
// 
// _______________________________________________________
u8 file_exists(str* filename)
{
   assertret0(filename, nnn filename given to file_exists); 
   struct stat info;
   return (stat(getarray(filename), &info) == 0);
}
// _______________________________________________________
// file_size
// 
// _______________________________________________________
u64 file_size(str* filename)
{
   assertret0(filename, nnn filename given to file_size); 
   struct stat info;
   stat(getarray(filename), &info);
   return info.st_size;
}
// _______________________________________________________
// file_destroy
// 
// _______________________________________________________
stinl u8 file_destroy(str* filename)
{
   if (!max_path_supported)
   {
      max_path_supported = pathconf(ROOT_DIRSTR, _PC_PATH_MAX);
   }
   assertret0(filename, nnn filename in file_destroy);
   assertret0(!isemptystr(filename), empty filename in file_destroy);
   assertret0(getlen(filename) <= max_path_supported, filepath longer than 'max_path_supported' in file_destroy)

   // =-> add support for file overwriting
   // =-> if (paranoid)
   // =-> file_scrub()...

   // =-> destroy link to the file 
   register int success = unlink(getarray(filename));
   
   #ifdef PRINTDEBUG
      sayline({}{}{}{}{}{}{}{}{});
      explain(%s : %s\n, "destroy", getarray(filename));
      explain(%s : %d\n, "error", errno);
      sayline({}{}{}{}{}{}{}{}{});
   #endif
   
   // =-> if unlinking failed
   assertret0(success != UNLINK_FAILED, open error: );
   
   return 1;
}



#endif // FILES_H
