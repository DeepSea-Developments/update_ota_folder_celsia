
#include "uSD.h"



FileStore FSlog;
void uSD_init(FileStore FS){

    /*SD SETUP**********************************************************/
    
    FS.Init(); // Initialization of FileStore object for file manipulation
    // Create/Open file "SerialRead"
    FS.CreateNew("0:","payload");
    

    FS.Close();
}

void uSD_events_init(){
    SD.Init(); // Initialization of HSCMI protocol and SD socket switch GPIO (to adjust pin number go to library source file - check Getting Started Guide)
    FSlog.Init(); // Initialization of FileStore object for file manipulation
    FSlog.CreateNew("0:","events");
}

int uSD_save_data(FileStore FS, char *mqtt_payload, unsigned long int msgcount){
    watchdogReset();
    static int cntMSG =0;
    int SDpos = 0;
    cntMSG++;
    char dataSD[1000];
    FS.Open("0:","logs",true);
    FS.Seek(FS.Length());
    SDpos=FS.Length();
    //snprintf(dataSD,sizeof(dataSD),"EN SD ES El %d : %s\n",msgcount,mqtt_payload);
    snprintf(dataSD,sizeof(dataSD),"%s\n",mqtt_payload);
    FS.Write(dataSD);
    FS.Close();
    return SDpos;
}

int uSD_save_data_events(char *msg_event, char *mqtt_payload){
    watchdogReset();
    char dataSD[1000];
    FSlog.Open("0:","events",true);
    FSlog.Seek(FSlog.Length());
    snprintf(dataSD,sizeof(dataSD),"%s  -- MSG: %s\n",msg_event,mqtt_payload);
    FSlog.Write(dataSD);
    FSlog.Close();
    watchdogReset();
    return 1;
}

void uSD_read_data(FileStore FS, char *mqtt_payload){

}