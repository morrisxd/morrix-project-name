#!/bin/bash 


targetLoc="216.241.237.121:dps/"
sourceLoc="/public/data/projects/winpath/"
# $?=1
ls
while [ $? -gt 0 ]
do
ls -l
# rsync -av versions/ 216.241.237.121:dps/
done

rsync -av /public/data/projects/winpath/sw_docs ${targetLoc}
rsync -av /public/data/projects/winpath/Standards ${targetLoc}
rsync -av ${sourceLoc}wddi/versions/186? ${targetLoc}wddi
rsync -av ${sourceLoc}wddi/versions/185? ${targetLoc}wddi
rsync -av ${sourceLoc}wddi/versions/153? ${targetLoc}wddi
