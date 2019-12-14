#include "../oraculo.h"
#include "task_handler.h"

OBD PID;
CAN_FRAME incoming;
GPS data_gps;
FileStore FS;
/*TASK HANDLERS*********************************************/
// Declare a semaphore handle.
SemaphoreHandle_t sem;
TaskHandle_t xcan_bus_filter_task, xgprs_send_task, xgps_adquisition;
// interval between points in units of 1000 usec
const uint16_t intervalTicks = 1000;
bool uSD_init_flag = false;
char unsend_mqtt_payload[1000];
char buff[1000];
char logs[10]={"logs"};
char elogs[10]={"eventlogs"};
static unsigned long lastTime1 = 0;
static unsigned long lastTime2 = 0;
/***********************************************************/


//------------------------------------------------------------------------------
/*
 * Thread can_bus_filter_task, read and filters can-bus data from the vehicule.
 */
// Declare the thread function for thread 1.
static void can_bus_filter_task(void* arg){
        
   while (1) {
           watchdogReset();
#ifdef RENUALT_KANGOO_ZE
        if ((millis() - lastTime1) > 40){
                lastTime1 = millis();
                sendData1_can_bus(41);    
        }
        if ((millis() - lastTime2) > 40){
                lastTime2 = millis();
                sendData2_can_bus();    
        }
#endif
    if (Can0.available() > 0) {
      Can0.read(incoming); 
#ifdef MITSUBISHI_I_MIEV
        PID_Filter_imiev(incoming,PID);
#endif

#ifdef RENUALT_KANGOO_ZE
        PID_Filter_kangoo(incoming,PID);
#endif

#ifdef BUS_YUTONG
        PID_Filter_yutong(incoming,PID);
#endif

    }
    
    xSemaphoreGive(sem);
  }
}





static void gprs_send_task(void* arg){
    // wait until time for next data point
    TickType_t ticks = xTaskGetTickCount();
        
    while (1) {
        watchdogReset();
        vTaskDelayUntil(&ticks, intervalTicks);
        vTaskSuspend(xcan_bus_filter_task);
        vTaskSuspend(xgps_adquisition);
        
#if DEBUG
        SerialMon.println("HOLA ENTRO EL TIMER");
#endif
        static int state = CONNECTED;
        static int lastState = CONNECTED;
        static int last_msg_send = 0;
        static int SDpos = 0;
        static int lastSDpos = 0;
        static int cnt_unsend = 0;
        static int max_unsend = 0;
        static bool still_unsend_msg = false;
        static int cnt_sec = 0;
        static int cnt_sec_reconnected = 0;
        static bool reconnected_flag = false;
        static bool saving_sd_disc = false;
        static unsigned long int cnt_msg=0;
        static int signal_strenght = 0;
        static int SD_lenght_payload = 0;
        cnt_msg++;

        if (cnt_msg == 20){
                uSD_init(FS);
                uSD_init_flag = true;
        }
        signal_strenght = GPRS_signalStrenght();
        char mqtt_payload[1000];
        json_maker(PID, data_gps, mqtt_payload, cnt_msg,saving_sd_disc,signal_strenght);
        if(uSD_init_flag){
               SDpos = uSD_save_data(FS, mqtt_payload,cnt_msg);
               SerialMon.print("posicion en sd: ");
               SerialMon.println(SDpos);
        }

        /*pinMode(Button1, INPUT);

        if(digitalRead(Button1) == HIGH){
                state = CONNECTED;
                SerialMon.println("CONECTADO");
        }else if(digitalRead(Button1) == LOW)
        {
                state = DISCONNECTED;
                SerialMon.println("DESCONECTADO");
        }*/

        /*if(GPRS_connected()){
                SerialMon.println("REAL CONECTADO");
        }else if(!GPRS_connected())
        {
                
                SerialMon.println("REAL DESCONECTADO");
        }*/

        
        if(GPRS_connected()){
                state = CONNECTED;
                SerialMon.println("CONECTADO");
        }else
        {
                state = DISCONNECTED;
                SerialMon.println("DESCONECTADO");

        }

        if((lastState == CONNECTED) && (state == CONNECTED)){
                saving_sd_disc = false;
                GPRS_send_mqtt(mqtt_payload);
                if(reconnected_flag){
                        cnt_sec_reconnected++;
                        if(cnt_sec_reconnected>30){
                                if(still_unsend_msg && uSD_init_flag){
                                        cnt_sec++;
                                        SerialMon.println("AQUI");
                                        if(cnt_sec == 4){
#if DEBUG
                                                SerialUSB.println("RECONECTADO ");
                                                SerialUSB.print("Reading from file: ");
#endif
                                                FS.Open("0:",logs,false);
                                                for(lastSDpos;lastSDpos<FS.Length();){
                                                        FS.Seek(lastSDpos); // Go to position 0 in file - beginning of the file
                                                        unsigned int i=0; // counter so we know when to stop reading from file
                                                        char b; // We will read one byte at the time
                                                        
                                                        
                                                        uint32_t t1 = millis();
                                                        do
                                                        {
                                                                FS.Read(b); // Read one byte from file and store it in "b"
                                                                if (b == '\n') break;
                                                                buff[i]=b;
                                                                i++;
                                                                
                                                        } while ((i<FS.Length()) || (millis() - t1 < 5000));
                                                        buff[i++]=NULL;
                                                        lastSDpos=FS.Position();
                                                        snprintf(unsend_mqtt_payload,sizeof(unsend_mqtt_payload), "%s",buff);
                                                        GPRS_send_mqtt(unsend_mqtt_payload);
#if DEBUG
                                                        SerialMon.println(unsend_mqtt_payload);
#endif

                                                        cnt_unsend--;
                                                        max_unsend++;
                                                        if(max_unsend >= 10){
                                                                max_unsend=0;
                                                                cnt_sec=0;
                                                                still_unsend_msg = true;
                                                                break;
                                                        }
                                                }
                                                max_unsend=0;
                                                if(last_msg_send  < cnt_unsend-1){
                                                        still_unsend_msg = true;
                                                }else
                                                {
                                                        cnt_unsend = 0;
                                                        still_unsend_msg = false;
                                                        cnt_sec=0;
                                                        saving_sd_disc = false;
                                                        cnt_sec_reconnected = 0;
                                                }
                                        }
                                }
                        }
                }

        }
        else if (lastState == DISCONNECTED && state == CONNECTED && uSD_init_flag){
#if DEBUG
                SerialUSB.println("RECONECTADO ");
                SerialUSB.print("Reading from file: ");
                uSD_save_data_events("Reconnected",mqtt_payload);
#endif
                reconnected_flag = true;
                still_unsend_msg = true;
                cnt_unsend = cnt_msg;      
        }
        else if (lastState == CONNECTED && state == DISCONNECTED)
        {
                if(!still_unsend_msg){
                        lastSDpos = SDpos-(SDpos - SD_lenght_payload)*4;
                }
                uSD_save_data_events("Disconnected",mqtt_payload);
                saving_sd_disc = true;
                last_msg_send = cnt_msg;
        }

        lastState = state;
        SD_lenght_payload = SDpos;
#if DEBUG
        SerialMon.println(cnt_msg);
        SerialMon.println(mqtt_payload);
#endif 
        
       // while(1);
       vTaskResume(xcan_bus_filter_task);
       vTaskResume(xgps_adquisition);
    }
}




//------------------------------------------------------------------------------
/*
 * Thread gps_adquisition, read GPS data: latitude, longitude, date-time.
 */
// Declare the thread function for thread 2.
static void gps_adquisition(void* arg){
        
    while (1) {
            watchdogReset();
     // Wait for signal from thread 1.
        xSemaphoreTake(sem, portMAX_DELAY);
        if (SerialGPS.available() > 0){
            GPS_get_info(&data_gps.latitud, &data_gps.longitud, &data_gps.mes, &data_gps.dia, &data_gps.ano, &data_gps.hora, &data_gps.minuto, &data_gps.segundo, &data_gps.num_sats, &data_gps.validDate, &data_gps.validGps);    
        }   
    }
}
int watchdogTime = 16000;
/*void watchdogSetup(void)
{
// do what you want here
}*/

void task_handler_init(void){
    /*TASK SETUP************************************************************/
    portBASE_TYPE s1, s2, s3;
    // initialize semaphore
    sem = xSemaphoreCreateCounting(1, 0);

    s1 = xTaskCreate(can_bus_filter_task, NULL, 1024, NULL, 2, &xcan_bus_filter_task);
    s2 = xTaskCreate(gprs_send_task, NULL, 1024, NULL, 2, &xgprs_send_task);
    s3 = xTaskCreate(gps_adquisition, NULL, 1024, NULL, 3, &xgps_adquisition);

    // check for creation errors
    if (sem== NULL || s1 != pdPASS || s2 != pdPASS || s3 != pdPASS) {
#if DEBUG
        SerialMon.println(F("Creation problem"));
#endif
        while(1);
    }
    // start scheduler
    watchdogEnable(watchdogTime);
    vTaskStartScheduler();
    
#if DEBUG
    SerialMon.println("Insufficient RAM");
#endif
    while(1);
}