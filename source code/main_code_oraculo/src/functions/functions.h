#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#include "../oraculo.h"

void fix_timestamp(GPS data_gps);
void json_maker(OBD PID, GPS data_gps, char *mqtt_payload, unsigned long int msg_count, bool SD_data, int signals);
void uSD_save_data(char *mqtt_payload);
void sendData1_can_bus(int code);
void sendData2_can_bus();
void PID_Filter_kangoo(CAN_FRAME incoming, OBD PID);
void PID_Filter_yutong(CAN_FRAME incoming, OBD PID);
void PID_Filter_imiev(CAN_FRAME incoming, OBD PID);

#endif	/* FUNCTIONS_H */