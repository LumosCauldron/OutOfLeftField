///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
#ifdef YANGCODE
   #include "include/yang/lamplight.h"
   #include "include/yang/winfiles.h"
   #define testfile "\\??\\test\\" //...check this
#elif YINCODE
   #include "include/yin/lamplight.h"
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
   
   STREAM portal1 = file_open(oldf, readmode);
   assertret(portal1 > INVALID_STREAM, file_open returned invalid stream for old file);
   STREAM portal2 = file_open(newf, createmode);
   assertret(portal2 > INVALID_STREAM, file_open returned invalid stream for new file);
   
   str* holdfile = hbuffer(filesz);
   explain(!!! File size is %lu bytes. !!!\n, filesz);
   
   if (Lamp(portal1, getarray(holdfile), getlen(holdfile), nullptr, ALIGNED_RATE, t1))
   {
      sayline(Read this file successfully.);
   }
   else
   {
      sayline(Did NOT read this file successfully.);
   }
   
   if (Light(portal2, getarray(holdfile), getlen(holdfile), nullptr, ALIGNED_RATE, t1))
   {
      ftruncate(portal2, filesz);
      sayline(Wrote this file successfully.);
   }
   else
   {
      sayline(Did NOT write this file successfully.);
   }
   
   freestr(&holdfile);
}

// ====================================================
// ====================================================
int main()
{  
   str* oldfile = datastr(png);
   str* newfile = datastr(pngcopy);
   
   copyfile(oldfile, newfile);
   
   RETURN;
}
// ====================================================
// ====================================================
