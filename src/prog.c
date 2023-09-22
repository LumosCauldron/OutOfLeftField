#include "stream.h"

int main()
{
   #ifdef YINCODE
      str* test = datastr("/home/servant/Documents/OutOfLeftField/test/data.txt");
   #elif YANGCODE
      str* test = datastr("test\\data.txt");
   #endif 
   
   //file_close(file_open(test, createmode));
   
   file_destroy(test);
   
   RETURN;
}

/* */
