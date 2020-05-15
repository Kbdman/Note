#!/bin/bash
MODNAME=chardev
INSTALLED=$(lsmod|grep ${MODNAME} -c)
DEVENTRY=0
getmajor(){
    DEVENTRY=$(cat /proc/devices|grep ${MODNAME}|awk -F " " '{print $1}')
    return ${DEVENTRY}
}
echo ${INSTALLED} 
if [ "${INSTALLED}" == "1" ]
then
    getmajor
    rmmod ${MODNAME}
    echo "removed"
else 
    echo "installing"
    insmod ${MODNAME}.ko
    getmajor
    rm /dev/${MODNAME}
    echo ${DEVENTRY}
    `mknod -m 0666 /dev/${MODNAME} c ${DEVENTRY} 0`
fi