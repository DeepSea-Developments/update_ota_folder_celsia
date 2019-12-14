#ifndef ORACULO_H
#define	ORACULO_H


#include "variant.h"            
#include <due_can.h>
#include <FreeRTOS_ARM.h>
#include <TinyGPS++.h>
#include <Arduino_Due_SD_HSMCI.h>
#include <modules/GPRS.h>
#include <modules/GPS.h>
#include <modules/OBD.h>
#include <modules/uSD.h>
#include "task/task_handler.h"
#include "functions/functions.h"


#define SerialMon SerialUSB  //serial port for debugging
#define SerialGPS Serial3    //serial port for GPS module   
#define SerialAT Serial      //serial port fot GPRS module   

#define DEBUG 1     //DEBUG OPTION         

#endif	/* ORACULO_H */