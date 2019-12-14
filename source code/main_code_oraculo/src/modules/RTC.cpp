#include "RTC.h"

RTCDue rtc(XTAL);


void RTC_init(){
    rtc.begin(); // initialize RTC
    rtc.setTime(0, 0, 0);
    rtc.setDate(1, 12, 2019);
}

void RTC_sync(GPS data_gps){
    rtc.setTime(data_gps.hora, data_gps.minuto, data_gps.segundo);
    rtc.setDate(data_gps.dia, data_gps.mes, data_gps.ano);
}

void RTC_timestamp(char *timestamp){
    char timertc[25];
    char string_mes[6];
    char string_dia[6];
    char string_hora[6];
    char string_minuto[6];
    char string_segundo[6];
    if (rtc.getMonth() < 10) {
            snprintf(string_mes, sizeof(string_mes), "0%d",rtc.getMonth());
    }else
    {
            snprintf(string_mes, sizeof(string_mes), "%d",rtc.getMonth());
    }

        if (rtc.getDay() < 10) {
            snprintf(string_dia, sizeof(string_dia), "0%d",rtc.getDay());
    }else
    {
            snprintf(string_dia, sizeof(string_dia), "%d",rtc.getDay());
    }

        if (rtc.getHours() < 10) {
            snprintf(string_hora, sizeof(string_hora), "0%d",rtc.getHours());
    }else
    {
            snprintf(string_hora, sizeof(string_hora), "%d",rtc.getHours());
    }

        if (rtc.getMinutes() < 10) {
            snprintf(string_minuto, sizeof(string_minuto), "0%d",rtc.getMinutes());
    }else
    {
            snprintf(string_minuto, sizeof(string_minuto), "%d",rtc.getMinutes());
    }

        if (rtc.getSeconds() < 10) {
            snprintf(string_segundo, sizeof(string_segundo), "0%d",rtc.getSeconds());
    }else
    {
            snprintf(string_segundo, sizeof(string_segundo), "%d",rtc.getSeconds());
    }
    
    snprintf (timertc, sizeof(timertc),"'%d-%s-%sT%s:%s:%sZ'",rtc.getYear(),string_mes,string_dia,string_hora,string_minuto,string_segundo);
    strcpy(timestamp, timertc);

}
