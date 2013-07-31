#!/bin/bash

. /users/dengmorr/.bashrc



cd ~/wddk
accurev update
echo done >> done.log

cd ~/WDDS_4_3_X_DEV_morris_israel
accurev update

cd ~/WDDK_MAIN_DEV
accurev update

cd ~/WDDS_4_2_X_DEV_morris
accurev update

cd ~/WDDS_4_3_X_DEV_morris_israel
accurev update

cd ~/WDDS_MAIN_DEV_refapps_morris_3
accurev update
