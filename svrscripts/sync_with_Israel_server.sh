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

rsync -avz /public/data/projects/winpath/sw_docs ${targetLoc}
rsync -avz /public/data/projects/winpath/Standards ${targetLoc}
rsync -avz ${sourceLoc}wddi/versions/186? ${targetLoc}wddi
rsync -avz ${sourceLoc}wddi/versions/185? ${targetLoc}wddi
rsync -avz ${sourceLoc}wddi/versions/153? ${targetLoc}wddi
rsync -avz ${sourceLoc}wddi/versions/174? ${targetLoc}wddi
rsync -avz ${sourceLoc}wddi/versions/177? ${targetLoc}wddi
rsync -avz ~/WDDS_MAIN_DEV_refapps_morris_2 ${targetRoot}
rsync -avz ~/WDDS_MAIN_DEV_refapps_morris_3 ${targetRootRD}
rsync -avz ~/WDDS_4_2_X_DEV_morris ${targetRootRD}
