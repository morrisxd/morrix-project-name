#!/bin/sh

name="mmap_drv"
device="wp_siu"
module="wp_mmap_drv.ko"

echo -ne "Loading WinPath mmap driver ... "

/sbin/insmod $module $* || exit 1

major=`awk "\\$2==\"$name\" {print \\$1}" /proc/devices`

wp_major=`awk '\$2=="mmap_drv" {print \$1}' /proc/devices`
wp_i=0
while [ $wp_i -lt 24 ]
do
 wp_j=`expr $wp_i + 1 `
 if [ ! -e /dev/wp_bank${wp_j} ]; then
   mknod /dev/wp_bank${wp_j} c ${wp_major} ${wp_i}
   if [ $? -ne 0 ]; then
     echo "    /dev/wp_bank${wp_j}: failed to create"
   fi
 fi
 wp_i=`expr $wp_i + 1 `
 unset wp_j
done
unset wp_i
unset wp_major
echo "done."

./xscheckwinmon
revstatus=$?

#echo "result = $revstatus"

if [ $revstatus -eq 1 ]; then
  echo "Error while checking revision"
  exit 1
fi

if [ "$revstatus" -eq "0" ]; then
  echo "Firmware is up-to-date"
  exit 0
fi

if [ "$revstatus" -eq "2" ]; then
  echo -ne "Updating I2C, please wait 20 seconds ... "
  ./xsprogi2c i2c_20111125.bin
  ./xsreset
  exit 0
fi

if [ "$revstatus" -eq "3" ]; then
  echo -ne "Updating Flash, please wait 20 seconds ... "
  ./xsprogflash flash_20120903.bin
  ./xsreset
  exit 0
fi 

if [ "$revstatus" -eq "4" ]; then
  echo -ne "Updating I2C, please wait 20 seconds ... "
  ./xsprogi2c i2c_20111125.bin
  echo -ne "Updating Flash, plase wait 20 seconds ... "
  ./xsprogflash flash_20120903.bin
  ./xsreset
  exit 0
fi

if [ "$revstatus" -eq "5" ]; then
  echo "### WARNING: Not a WinPath3 Rev. B2, skipping update."
  exit 1
fi

exit 0
