n=0
{
   while read LINE
   do
      n=`expr $n + 1` 
      echo "$n) $LINE"
   done
} < mylog

