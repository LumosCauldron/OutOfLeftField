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
u64 encryptfunction_space = 49;
u64 encryptfunction(void* stuff)
{
   return 0;
}

void copyfile(str* oldf, str* newf)
{
   u64 filesz = file_size(oldf);
   assertret(filesz, file_size returned zero);
   
   STREAM portal1 = file_open(oldf, readmode);
   assertret(portal1 > invalid_stream, file_open returned invalid stream for old file);
   STREAM portal2 = file_open(newf, createmode);
   assertret(portal2 > invalid_stream, file_open returned invalid stream for new file);
   
   str* holdfile = hbuffer(increase_to_aligned_amount(filesz));
   explain(!!! File    size    is  %lu bytes. !!!\n, filesz);
   explain(!!! Rate for reading is %lu bytes. !!!\n, fast_aligned_read_rate);
   explain(!!! Rate for writing is %lu bytes. !!!\n, fast_aligned_write_rate);
   
   nompkg processing_module = { encryptfunction, encryptfunction_space };
   
   if (readfile(portal1, getarray(holdfile), filesz, &processing_module, &portal2, fast_aligned_read_rate, direction_and_type(nextop, readop)))
   {
      sayline(Read this file successfully.);
   }
   else
   {
      sayline(Did NOT read this file successfully.);
   }
   
   if (writefile(portal2, getarray(holdfile), filesz, fast_aligned_write_rate))
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
   str* oldfile = datastr("test/randomdata");
   str* newfile = datastr("test/randomdata_copy");
   
   copyfile(oldfile, newfile);
   
   RETURN;
}
// ====================================================
// ====================================================
