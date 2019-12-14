#ifndef RTC_H
#define	RTC_H

#include "../oraculo.h"



void RTC_init();

void RTC_sync(GPS data_gps);

void RTC_timestamp(char *timestamp);



#endif	/* RTC_H */
