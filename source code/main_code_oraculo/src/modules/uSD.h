#ifndef USD_H
#define	USD_H

#include "../oraculo.h"



void uSD_init(FileStore FS);

void uSD_events_init();

int uSD_save_data(FileStore FS, char *mqtt_payload, unsigned long int msgcount);

int uSD_save_data_events(char *msg_event, char *mqtt_payload);

void uSD_read_data(FileStore FS, char *mqtt_payload);


#endif	/* uSD_H */
