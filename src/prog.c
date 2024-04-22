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
#define readfile(oldf, newf) encryptfile((oldf), (newf), nullptr)
void encryptfile(str* oldf, str* newf, CryptoGemStone* gem)
{
   u64 filesz = file_size(oldf);
   
   STREAM in = file_open(oldf, readmode);
   STREAM out = file_open(newf, createmode);
   
   ftruncate(out, filesz); // get harddisk space mapped out
   if (read_forward(in, filesz, out, gem))
   {  // truncate to the correct size
      ftruncate(out, filesz); 
      sayline(Read this file successfully.);
   }
   else
   {
      sayline(Did NOT read this file successfully.);
   }
   
   file_close(in);
   file_close(out);
}

#define readfilebackwards(oldf, newf) decryptfile((oldf), (newf), nullptr)
void decryptfile(str* oldf, str* newf, CryptoGemStone* gem)
{
   u64 filesz = file_size(oldf);
   
   STREAM in = file_open(oldf, readmode);
   STREAM out = file_open(newf, createmode);
   
   ftruncate(out, filesz); // get harddisk space mapped out
   if (read_backward(in, filesz, out, gem))
   {  // truncate to the correct size
      ftruncate(out, filesz); 
      sayline(Read this file successfully.);
   }
   else
   {
      sayline(Did NOT read this file successfully.);
   }
   
   file_close(in);
   file_close(out);
}
#include <stdio.h>
/*
CryptoGemStone gem1;
cryptokey(testKey1->array, &gem1);
encryptstr(field1->array, getlen(field1), &gem1);
decryptstr(field1->array, getlen(field1), &gem1);
*/
// ====================================================
// ====================================================
int main()
{  
   str* mansees1 = datastr("test/randomdata");
   str* Godsees = datastr("test/randomdata_copy");
   str* mansees2 = datastr("test/randomdata_copy_again");
   str* key = datastr("thisisa49bytekeyworksverynicely to God betheglory");

   CryptoGemStone gem;
   cryptokey(getarray(key), &gem);

   encryptfile(mansees1, Godsees, &gem);
   decryptfile(Godsees, mansees2, &gem);
   
   RETURN;
}
// ====================================================
// ====================================================
