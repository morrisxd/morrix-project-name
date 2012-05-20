#!/bin/bash
i=20
while [ $i -le 30 ]
do
	echo "2003/01/0$i"
	grep "2003/01/$i" sh600628_$i.txt > ./txt2/$i.txt
	i=`expr $i + 1`
done

