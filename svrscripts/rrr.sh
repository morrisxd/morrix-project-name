#!/bin/bash 


targetLoc="216.241.237.121:dps/"
targetRoot="216.241.237.121:."
targetRootRD="216.241.237.121:RD/"
targetProj="216.241.237.121:./proj/"
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
rsync -av ${sourceLoc}wddi/versions/174? ${targetLoc}wddi
rsync -av ${sourceLoc}wddi/versions/177? ${targetLoc}wddi
rsync -av ~/WDDS_MAIN_DEV_refapps_morris_2 ${targetRoot}
rsync -av ~/WDDS_MAIN_DEV_refapps_morris_3 ${targetRootRD}
# rsync -av ~/WDDS_4_2_X_DEV_morris ${targetProj}
