#include "stream.h"

int main()
{
   const int num = 270;
   u8* fileName = charspace(270); // 300 'a' characters + null terminator

    // Initialize the file name with 300 'a' characters
    for (int i = 0; i < num; i++) 
        fileName[i] = 'a';

   #ifdef YINCODE
      str* test = datastr("/home/servant/Documents/OutOfLeftField/test/data.txt");
   #elif YANGCODE
      str* test = datastr("\\\\?\\test\\");
   #endif 
   str* ultratest = appendstr(test, charstr(num, fileName));
   file_close(file_open(ultratest, createmode));
   file_destroy(ultratest);
   
   str* test1 = datastr("\\\\?\\test\\data.txt");
   file_close(file_open(test1, createmode));
   file_destroy(test1);
   
   file_close(file_open(charstr(num, fileName), createmode));
   file_destroy(charstr(num, fileName));
   
   str* test2 = datastr("test\\data.txt");
   file_close(file_open(test2, createmode));
   file_destroy(test2);
   
   freestr(&ultratest);
   RETURN;
}

/* */
