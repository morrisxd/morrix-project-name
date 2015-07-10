#!/bin/bash 
# ./fetchcmcc.sh

mybw=100
targetLoc="216.241.237.121:dps/"
targetWinmon="216.241.237.121:dps/winmon/"
targetRoot="216.241.237.121:."
targetRootRD="216.241.237.121:RD/"
sourceLoc="/public/data/projects/winpath/"
my_israel_elad="/users/dengmorr/elad/"




rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/.exrc ${targetRoot}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/.compiler2 ${targetRoot}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/bin ${targetRoot}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/scripts ${targetRoot}



rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDS_5_0_ALU_MNT_morris ${targetRootRD}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDS_5_1_RC3_b2031_2014_06_30_morris_self_stream ${targetRootRD}

rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDS_3_4_RC2_v967_fixed_morris ${targetRootRD}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}winmon/releases/5.1.11  ${targetWinmon}
# sourcef=/home/dengmorr/RD/WDDS_5_0_GCA_CMCC_PTN_morris/apps/refapps/ce_sh/*
# targetf=/users/dengmorr/WDDS_5_0_GCA_CMCC_PTN_morris/apps/refapps/ce/
# rsync -avz --progress --exclude=cscope* --bwlimit=100 sgh1sin01:${sourcef}  ${targetf}

# rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} sgh1sin01:./ericsson-spp-from-juhi /users/dengmorr/from_juhi
rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} ~/WDDS_4_3_2_4.3.2_b1864_2013_06_18_morris_asb ${targetRootRD}
rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} ~/WDDS_5_0_GCA_CMCC_PTN_morris ${targetRootRD}

rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} /users/eladk/Clock_rec_course ${targetRootRD}
rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} ~/WDDS_MAIN_DEV_PTP_FIBERHOME_morris ${targetRootRD}
rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} /users/eladsh/projects/winpath/WDDS_MAIN_DEV_A ${targetRootRD}
# rsync -avz --progress /users/eladsh/projects/winpath/WDDS_MAIN_DEV_A ${my_israel_elad}
rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} /users/eladk/*.xls ${targetRootRD}
rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} /users/eladsh/CLOCK_RECOVERY/TEST/test_equipment ${targetRootRD}

rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}bsps/mvl/monta-vista/40/mips ${targetRootRD}
# rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}bsps/mvl/monta-vista/40/mips/docs-pro-0501140.iso ${targetRootRD}
# rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}bsps/mvl/monta-vista/40/mips/src-pro-0501140.iso ${targetRootRD}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress /public/data/projects/winpath/sw_docs ${targetLoc}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress /public/data/projects/winpath/Standards ${targetLoc}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/186? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/185? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/153? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/174? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/17?? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/181? ${targetLoc}wddi
echo begin to sync versions
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/18?? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/19?? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}wddi/versions/22?? ${targetLoc}wddi
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}winutil ${targetLoc}
echo begin to sync MAIN_DEV_morris_3
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDS_MAIN_DEV_refapps_morris_3 ${targetRootRD}
echo begin to sync 4_3_x_dev_israel
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDS_4_3_X_DEV_morris_israel ${targetRoot}
echo begin to sync 4_2_x
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDS_4_2_X_DEV_morris ${targetRootRD}
echo begin to sync WDDK_MAIN_DEV
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ~/WDDK_MAIN_DEV ${targetRootRD}




# rsync -avz --progress dengmorr@216.241.237.121:.exrc ~
# /public/data/projects/winpath/winmon/releases

rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}winmon/releases/3.6* ${targetWinmon}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}winmon/releases/4.2* ${targetWinmon}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}winmon/releases/4.3* ${targetWinmon}
rsync -avz --exclude=cscope* --bwlimit=${mybw} --progress ${sourceLoc}winmon/releases/5.*  ${targetWinmon}

rsync -avz --progress --exclude=cscope* --bwlimit=${mybw} /public/data/projects/winpath/wddi/versions/1468 dengmorr@216.241.237.121:dps/wddi

