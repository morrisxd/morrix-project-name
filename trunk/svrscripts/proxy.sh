#!/bin/bash
myport=2001
$?=3

while [ $? -gt 0 ]
do
	ssh -C -g -D ${myport} ran1sw08
	if [ $? -gt 0 ]
	then
		sleep 10
		$?=3
	fi
done

