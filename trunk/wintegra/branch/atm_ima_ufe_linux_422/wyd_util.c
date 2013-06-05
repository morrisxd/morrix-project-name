#ifndef __WYD_UTIL_C__
#define __WYD_UTIL_C__

unsigned char wyd_getchar (void)
{
   unsigned char comm = 0;

   do
   {
      comm = getchar ();
   }
   while (!(('z' >= comm && comm >= 'a') || ('0' <= comm && comm <= '9')));

   return comm;
}

#endif
