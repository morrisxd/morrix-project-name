#!/bin/bash
$?=1

while [ $? -gt 0 ]
do 
svn commit -m"clear"
done

