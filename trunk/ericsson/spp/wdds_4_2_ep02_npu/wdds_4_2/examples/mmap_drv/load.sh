#!/bin/sh

name="mmap_drv"
device="wp_siu"
module="wp_mmap_drv.ko"

/sbin/insmod $module $* || exit 1

major=`awk "\\$2==\"$name\" {print \\$1}" /proc/devices`

wp_major=`awk '\$2=="mmap_drv" {print \$1}' /proc/devices`
wp_i=0
while [ $wp_i -lt 24 ]
do
 wp_j=`expr $wp_i + 1 `
 if [ ! -e /dev/wp_bank${wp_j} ]; then
   mknod /dev/wp_bank${wp_j} c ${wp_major} ${wp_i}
   if [ $? -eq 0 ]; then
     echo "    /dev/wp_bank${wp_j}: created"
   else
     echo "    /dev/wp_bank${wp_j}: failed to create"
   fi
 fi
 wp_i=`expr $wp_i + 1 `
 unset wp_j
done
unset wp_i
unset wp_major
echo "Done."
