#!/bin/bash 

mybw=100
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

# rsync -avz --bwlimit=100 --progress ${sourceLoc}bsps/mvl/monta-vista/40/mips/docs-pro-0501140.iso ${targetRootRD}
# rsync -avz --bwlimit=100 --progress ${sourceLoc}bsps/mvl/monta-vista/40/mips/src-pro-0501140.iso ${targetRootRD}
rsync -avz --bwlimit=100 --progress /public/data/projects/winpath/sw_docs ${targetLoc}
rsync -avz --bwlimit=100 --progress /public/data/projects/winpath/Standards ${targetLoc}
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/186? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/185? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/153? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/174? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/17?? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/181? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/18?? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}wddi/versions/19?? ${targetLoc}wddi
rsync -avz --bwlimit=100 --progress ${sourceLoc}winutil ${targetLoc}
rsync -avz --bwlimit=100 --progress ~/WDDS_MAIN_DEV_refapps_morris_2 ${targetRoot}
rsync -avz --bwlimit=100 --progress ~/WDDS_MAIN_DEV_refapps_morris_3 ${targetRootRD}
echo begin to sync 4_2_x
rsync -avz --bwlimit=100 --progress ~/WDDS_4_2_X_DEV_morris ${targetRootRD}
