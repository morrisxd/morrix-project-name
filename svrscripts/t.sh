#!/bin/bash 


targetLoc="216.241.237.121:dps/"
targetRoot="216.241.237.121:."
targetRootRD="216.241.237.121:RD/"
targetProj="216.241.237.121:./proj/"
sourceLoc="/public/data/projects/winpath/"

rsync -avz ~/hello.c ${targetRoot}
