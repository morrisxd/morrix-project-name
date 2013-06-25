#!/bin/bash 
$?=1

while [ $? -gt 0 ]
do
rsync -av versions/ 216.241.237.121:dps/
done

rsync -av /public/data/projects/winpath/sw_docs 216.241.237.121:dps

