#include "../oraculo.h"
#include "GPS.h"

TinyGPSPlus gps;      //GPS class


void GPS_init(void){
    // Set GPS module baud rate
    SerialGPS.begin(GPS_BAUDRATE);

}

void GPS_get_info(double *latitude, double *longitude, uint8_t *month, uint8_t *day, uint16_t *year, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *num_sats, bool *validDate, bool *validGPS){

  if (SerialGPS.available() > 0){
    if (gps.encode(SerialGPS.read())){
#if DEBUG
        SerialMon.println("ENTRO GPS");
#endif
      if (gps.location.isValid()){
        *latitude = gps.location.lat();
        *longitude = gps.location.lng();
        *validGPS = true;
      }
      else
      {
        *validGPS = false;
#if DEBUG
        SerialMon.print("INVALID");
#endif
      }
      if (gps.date.isValid())
      {
        *month = gps.date.month();
        *day = gps.date.day();
        *year = gps.date.year();
        *validDate = true;
      }
      else
      {
        *validDate = false;
#if DEBUG
        SerialMon.print("INVALID");
#endif
      }
      if (gps.time.isValid()){

        *hour = gps.time.hour();
        *minute = gps.time.minute();
        *second = gps.time.second();
      
      }
      else
      {
#if DEBUG
        SerialMon.print("INVALID");
#endif
      } 
      if (gps.satellites.isValid()){
        *num_sats = gps.satellites.value();
      }
      else
      {
#if DEBUG
        *num_sats = 0;
        SerialMon.print("INVALID");
#endif
      }  
    }
  }    
}

