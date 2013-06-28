#!/bin/bash 


targetLoc="216.241.237.121:dps/"
targetRoot="216.241.237.121:."
targetRootRD="216.241.237.121:RD/"
sourceLoc="/public/data/projects/winpath/"
# $?=1
ls
while [ $? -gt 0 ]
do
ls -l
# rsync -av versions/ 216.241.237.121:dps/
done

# rsync -avz -n ~/WDDS_4_2_X_DEV_morris ${targetRootRD}
rsync -avz ${sourceLoc}winutil ${targetLoc}
