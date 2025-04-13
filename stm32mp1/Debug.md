# Debug
The board can be debugged with openocd and gdb in sdk.            
The openocd connects to the board and provides gdb server on local.     
Then gdb can connect to the gdb server to debug the board.      
ST provides 2 scripts to help debugging:
+ [Setup.gdb](https://github.com/STMicroelectronics/wiki-stm32mp-addons/blob/main/gdb-scripts/Setup.gdb)    gdb scripts to setup the gdb
+ [Path_env.py](https://github.com/STMicroelectronics/wiki-stm32mp-addons/blob/main/gdb-scripts/Path_env.py) configurate script for symbols and sources, called by Setup.gdb        

These two file can be adapted accordingly.

## start Debug
Connect openocd to the board in first console
```
source ${sdk}
openocd -f /usr/share/openocd/scripts/board/stm32mp15x_dk2.cfg
```
To debug the U-boot, the boad should be reset to console mode
```
Loading Environment from MMC... OK                                              
In:    serial                                                                   
Out:   serial                                                                   
Err:   serial                                                                   
Net:   eth0: ethernet@5800a000                                                  
No EFI system partition                                                         
No EFI system partition                                                         
Failed to persist EFI variables                                                 
Hit any key to stop autoboot:  0                                                
STM32MP> 
```
Start gdb
```
source ${sdk}
${gdb in sdk} -x Setup.gdb
```
Then the board will be reset and gdb will be attached in next