///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
#ifdef YANGCODE
   #include "include/yang/rw.h"
   #include "include/yang/winfiles.h"
   #define testfile "\\??\\test\\" //...check this
#elif YINCODE
   #include "include/yin/rw.h"
   #include "include/yin/nixfiles.h"
   #define txt       "/home/servant/Documents/OutOfLeftField/test/data.txt"
   #define txtcopy   "/home/servant/Documents/OutOfLeftField/test/data_copy.txt"
   #define jpg       "/home/servant/Documents/OutOfLeftField/test/data.jpg"
   #define jpgcopy   "/home/servant/Documents/OutOfLeftField/test/data_copy.jpg"
   #define png       "/home/servant/Documents/OutOfLeftField/test/data.png"
   #define pngcopy   "/home/servant/Documents/OutOfLeftField/test/data_copy.png"
#else
   #error WHATMODE
#endif
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void copyfile(str* oldf, str* newf)
{
   u64 filesz = file_size(oldf);
   assertret(filesz, file_size returned zero);
   
   STREAM in = file_open(oldf, readmode);
   assertret(in > invalid_stream, file_open returned invalid stream for old file);
   STREAM out = file_open(newf, createmode);
   assertret(out > invalid_stream, file_open returned invalid stream for new file);
   
   if (readit(in, filesz, out))
   {
      sayline(Read this file successfully.);
   }
   else
   {
      sayline(Did NOT read this file successfully.);
   }
}

// ====================================================
// ====================================================
int main()
{  
   str* oldfile = datastr("test/randomdata");
   str* newfile = datastr("test/randomdata_copy");
   
   copyfile(oldfile, newfile);
   
   RETURN;
}
// ====================================================
// ====================================================
