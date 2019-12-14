#ifndef GPS_H
#define	GPS_H

#define GPS_BAUDRATE 9600   //Serial GPS speed

//Class: GPS
//A class for handling incomming GPS data.
class GPS{
public:
    double latitud;
    double longitud;
    uint8_t mes;
    uint16_t ano;
    uint8_t dia;
    uint8_t hora;
    uint8_t minuto;
    uint8_t segundo;
    uint8_t num_sats;
    bool validDate;
    bool validGps;
};

//PROTOTYPES FUNCTIONS
void GPS_init(void);
void GPS_get_info(double *latitude, double *longitude, uint8_t *month, uint8_t *day, uint16_t *year, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *num_sats, bool *validDate, bool *validGPS);

#endif	/* GPS_H */