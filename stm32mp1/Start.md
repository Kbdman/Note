# STM32MP157d-DK1
## Get start
![stm32mp157d](./stm32mp1.jpg)
Flash:
+ Download STM32CubeProgrammer and flash package from ST homepage.        
+ Unzip the flash packge.
+ Go to the directory contains the images and layout
+ swith 2 boot switchs to off      
+ Connect OTG to host PC
+ connect Powersupply
+ press reset
+ flash the board with a corresponding layout with command
  +  ```STM32_Programmer_CLI -c port=usb1 -w flashlayout_st-image-weston/optee/FlashLayout_sdcard_stm32mp157d-dk1-optee.tsv```
+  wait for finish

Boot:
+ swith 2 boot switchs to on
+ connect to serial output with command like
  + ```minicom -D /dev/ttyACM0```
+ press reset

## Flash Layout
STM32 using a .tsv file to define a flash configuration including flash which file to which part of which device.
```
#Opt	Id	Name	Type	IP	Offset	Binary
-	0x01	fsbl-boot	Binary	none	0x0	tf-a-stm32mp157d-dk1-optee-programmer-usb.stm32
-	0x03	fip-boot	FIP	none	0x0	fip-stm32mp157d-dk1-optee-programmer-usb.bin
P	0x04	fsbl1	Binary	mmc0	0x00004400	tf-a-stm32mp157d-dk1-optee-sdcard.stm32
PD	0x06	metadata1	FWU_MDATA	mmc0	0x00084400	metadata.bin
P	0x08	fip-a	FIP	mmc0	0x00104400	fip.bin

```
This column **Opt** defines the operation to the specified partition and **Id** identities an unique download process.

Some **Id** are reservered for specific usaged like:
+ 0x01 for image containing FSBL,it will be loaded by ROM code in ram and the file should have a stm32 header
+ 0x03 for image containing SSBL,it will be loaded by FSBL in external RAM for serial boot: it is a FIP binary with OpenSTLinux 
  
There are some requirements on **name**  for GPT partitions.The ROM code might use them to identity which partition contains which content, like "fsbl","fsbl1","fip","fip-a"