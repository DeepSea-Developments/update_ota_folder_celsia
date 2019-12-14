# update_ota_folder_celsia

This repository is used to remote  upgrade of oraculo devices of celsia

Please overwrite the firmware.bin in the specific folder  (HDX148 for example) with the new firmware for that device

CHECKLIST:

>sustitute the folder content at this path C:\Users\LENOVO\.platformio\packages\tool-bossac
with new bossac folder 
( this new version of bossac able the offset in addres flashing , very import to still programing over usb without
overwrite the ota booloader) 

>modify the main.py in C:\Users\LENOVO\.platformio\platforms\atmelsam\builder with the folder builder modified
(change the builder command to include the offset , IMPORTANT: this new script will loking for a directive in
platformio.ini

>last step is copy the linker script and call it from platformio ex. board_upload.offset_address=0x00040000 
use this offset when you gonna program with usb cable 

>change in Ring Buffer.h del core del sam , serial buffer size by 1024 or more
(this is for avoid the overflow of uart at download step executed by the OTA Bootloader)

LAST INFORMATION: 
There are two linker files  (.ld) for each "OTA bootloader" and "application" , these files are already used in
each project , check "bootloader_ota" and "main_code_oraculo" 
