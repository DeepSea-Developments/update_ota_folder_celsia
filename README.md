OTA BOOTLOADER  

This is an implemtentation repository of a custom ota bootloader based on gprs modem xbc-m5-ut-001 (bypass mode>) using the platformIO framework and the hardware macchine m2 for celsia DSD project named ORACULO


The repository is also used has storage of the firmwares of each devices.
Please overwrite the firmware.bin in the specific folder  (HDX148 for example) with the new firmware for that device

features:

* Identification of reset cause :

    hardware reset (power loss)
    app failure (watchdogg based)
    bootloader failure (watchdogg based)
    
* Shared variables  to extend the  reset reason detection to application

* Bootloader  network credentials (APN,MQTT,URL for firmware download) configurable

  These parameters are extracted of the app flash section , so the user can change it after booloader programing step
  
* Azure mqtt conectivity

  In case of many reset caused by the execution of the firmware downloaded , the ota bootloader will ask to the azure mqtt broker
  confirm a new firmware download.
  Each reset will be reported to the azure mqtt broker 
  In case of no app be detected  the ota will always ask to azure for a firmware download
  In case no confirmation of firmware download, the ota will jump to the current app  if exist , otherwise will contine asking
  The application must have a way to trigger (by user) a firmware download.the application example in the source forlder has this 
  already implemented
  
 * Valid commands from azure:
 { remot_cmd : 'yes' } -> to confirm an auto triggeredd upgrade procedure.
 { remot_cmd : 'upload_now' } -> trigger a update from the application runing  




IDE SETTINGS(PLATFORMIO - VS CODE

* sustitute the folder content at this path C:\Users\LENOVO\.platformio\packages\tool-bossac
with new bossac folder 
( this new version of bossac able the offset in addres flashing , very import to still programing over usb without
overwrite the ota booloader) 

* modify the main.py in C:\Users\LENOVO\.platformio\platforms\atmelsam\builder with the folder builder modified
(change the builder command to include the offset , IMPORTANT: this new script will loking for a directive in
platformio.ini

* last step is copy the linker script and call it from platformio ex. board_upload.offset_address=0x00040000 
use this offset when you gonna program with usb cable 

* change in Ring Buffer.h del core del sam , serial buffer size by 1024 or more
(this is for avoid the overflow of uart at download step executed by the OTA Bootloader)

LAST INFORMATION: 
There are two linker files  (.ld) for each "OTA bootloader" and "application" , these files are already used in
each project , check "bootloader_ota" and "main_code_oraculo" and used in the .ini file of platformIO for vars/constant sharing and flash offset.

