#ifndef FILES_H
#define FILES_H

#define _GNU_SOURCE // for other 'O_' file io options
#define _FILE_OFFSET_BITS 64 // for O_LARGEFILE file io option

// notes: 
// 1. improve =-> (void) __sseek((void *)fp, (0fpos_t)0, SEEK_END); maybe for O_APPEND functionality?
// 2. improve =-> give ability to change directories to avoid maximum supported path errors

// compatibility 
#ifndef O_DIRECT
   #define O_DIRECT 0 // future outlook =-> trying to avoid compilation error (we're probably compiling for Mac OSX)
#endif

// unix libs
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// types
typedef int MODE;
typedef i64 STREAM;

// modes/misc
#define maxname 255 // misc
#define slashstr "/" // misc
#define executemode 1
#define writemode 2
#define readmode 4 
#define readwrite 6
#define specialmode 7
#define createmode 8

// read/write speed
#define disk_block_size 4096
#ifndef EMBEDDED_MODE
   #define fast_aligned_rate ((u64)(0b00000001 << 30)) // ~2147mbs per read (hardcoded max speed)
#else
   #define fast_aligned_rate (disk_block_size) // ~4kbs per read (hardcoded max speed)
#endif
#define rwDBS disk_block_size
#define rwFAR fast_aligned_rate // aligned read/write rate (don't change)

#define invalid_stream -1
#define lseek_failed -1
#define read_failed -1
#define stat_failed -1
#define write_failed -1
#define unlink_failed -1

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
      max_path_supported = pathconf(slashstr, _PC_PATH_MAX);
   }
   
   arx(filename, nnn filename in file_open, invalid_stream);
   ar0(!isemptystr(filename), empty filename in file_open);
   arx(getlen(filename) <= max_path_supported, filepath longer than 'max_path_supported' in file_open, invalid_stream)
   // must be the same as file_destroy
   // DUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODEDUPCODE
   
   // =-> (write straight to disk linux:"O_DIRECT", osx:"fcntl(fd, F_NOCACHE, 1)"
   // =-> large files ok
   // =-> don't update date
   // =-> symlinks don't count
   // =-> ensure data don't care about metadata
   int genflags = O_LARGEFILE | O_NOATIME  | 
                          O_NOFOLLOW | O_DSYNC | O_DIRECT;
   
   // =-> when creating/createmode: replace already existing, write only then add the above
   int finalflags = O_CREAT | O_WRONLY | 
                           O_TRUNC | genflags;
                          
   int mode = S_ISVTX | S_IRWXU;
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
      default: arx(0, what mode is wanted to open the file, invalid_stream);
   }
   
   // actual open operation
   int fd = open(getarray(filename), finalflags, mode);
   
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
   aerx(fd != invalid_stream, open error: , invalid_stream);
   
   #ifdef IZANAMI
      if (fd != invalid_stream)  // same as using O_DIRECT but for OSX
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
   ar(stream >= 0, stream is less than zero in file_close... not closing this);
   
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
   ar0(filename, nnn filename given to file_exists); 
   struct stat info;
   return (stat(getarray(filename), &info) == 0);
}
// _______________________________________________________
// file_size
// 
// _______________________________________________________
u64 file_size(str* filename)
{
   ar0(filename, nnn filename given to file_size); 
   struct stat info;
   ar0(stat(getarray(filename), &info) != stat_failed, stat failed);
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
      max_path_supported = pathconf(slashstr, _PC_PATH_MAX);
   }
   ar0(filename, nnn filename in file_destroy);
   ar0(!isemptystr(filename), empty filename in file_destroy);
   ar0(getlen(filename) <= max_path_supported, filepath longer than 'max_path_supported' in file_destroy)

   // =-> add support for file overwriting
   // =-> if (paranoid)
   // =-> file_scrub()...

   // =-> destroy link to the file 
   int success = unlink(getarray(filename));
   
   #ifdef PRINTDEBUG
      sayline({}{}{}{}{}{}{}{}{});
      explain(%s : %s\n, "destroy", getarray(filename));
      explain(%s : %d\n, "error", errno);
      sayline({}{}{}{}{}{}{}{}{});
   #endif
   
   // =-> if unlinking failed
   ar0(success != unlink_failed, open error: );
   
   return 1;
}



#endif // FILES_H
