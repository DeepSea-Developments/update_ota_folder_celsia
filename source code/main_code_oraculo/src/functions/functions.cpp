#include "functions.h"
#include "../modules/RTC.h"
#include "../modules/uSD.h"
#include "../oraculo.h" 



void fix_timestamp(GPS data_gps){
        
        RTC_sync(data_gps);

}

void json_maker(OBD PID, GPS data_gps, char *mqtt_payload, unsigned long int msg_count, bool SD_data, int signals){

        char payload[1000];
        static bool firs_timesync_date = true;
        char string_frame_1[38];
        char string_frame_2[38];
        char string_frame_3[38];
        char string_frame_4[38];
        char string_frame_5[38];
        char string_frame_6[38];
        char string_frame_7[150];
        char string_frame_8[38];
        char string_frame_9[38];
        char string_frame_10[40];
        char string_frame_11[40];
        char string_frame_12[40];
        char timestamp[21];

        if ((((msg_count % 3600)==0) && (data_gps.validDate)) || ((firs_timesync_date) && (data_gps.validDate)) ){
                fix_timestamp(data_gps);
                firs_timesync_date=false;
        }
        RTC_timestamp(timestamp);

#ifdef MITSUBISHI_I_MIEV
        snprintf (string_frame_1, sizeof(string_frame_1), "ID1: '%03X', Data1: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID1,PID.frame_1.bytes[0],PID.frame_1.bytes[1],PID.frame_1.bytes[2],PID.frame_1.bytes[3],PID.frame_1.bytes[4],PID.frame_1.bytes[5],PID.frame_1.bytes[6],PID.frame_1.bytes[7]);
        snprintf (string_frame_2, sizeof(string_frame_2), "ID2: '%03X', Data2: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID2,PID.frame_2.bytes[0],PID.frame_2.bytes[1],PID.frame_2.bytes[2],PID.frame_2.bytes[3],PID.frame_2.bytes[4],PID.frame_2.bytes[5],PID.frame_2.bytes[6],PID.frame_2.bytes[7]);
        snprintf (string_frame_3, sizeof(string_frame_3), "ID3: '%03X', Data3: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID3,PID.frame_3.bytes[0],PID.frame_3.bytes[1],PID.frame_3.bytes[2],PID.frame_3.bytes[3],PID.frame_3.bytes[4],PID.frame_3.bytes[5],PID.frame_3.bytes[6],PID.frame_3.bytes[7]);
        snprintf (string_frame_4, sizeof(string_frame_4), "ID4: '%03X', Data4: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID4,PID.frame_4.bytes[0],PID.frame_4.bytes[1],PID.frame_4.bytes[2],PID.frame_4.bytes[3],PID.frame_4.bytes[4],PID.frame_4.bytes[5],PID.frame_4.bytes[6],PID.frame_4.bytes[7]);
        snprintf (string_frame_5, sizeof(string_frame_5), "ID5: '%03X', Data5: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID5,PID.frame_5.bytes[0],PID.frame_5.bytes[1],PID.frame_5.bytes[2],PID.frame_5.bytes[3],PID.frame_5.bytes[4],PID.frame_5.bytes[5],PID.frame_5.bytes[6],PID.frame_5.bytes[7]);
        snprintf (string_frame_6, sizeof(string_frame_6), "ID6: '%03X', Data6: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID6,PID.frame_6.bytes[0],PID.frame_6.bytes[1],PID.frame_6.bytes[2],PID.frame_6.bytes[3],PID.frame_6.bytes[4],PID.frame_6.bytes[5],PID.frame_6.bytes[6],PID.frame_6.bytes[7]);
        snprintf (string_frame_7, sizeof(string_frame_7), "ID7: '%03X', Data7: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID7,PID.frame_7.bytes[0],PID.frame_7.bytes[1],PID.frame_7.bytes[2],PID.frame_7.bytes[3],PID.frame_7.bytes[4],PID.frame_7.bytes[5],PID.frame_7.bytes[6],PID.frame_7.bytes[7]);
        snprintf (string_frame_8, sizeof(string_frame_8), "ID8: '%03X', Data8: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID8,PID.frame_8.bytes[0],PID.frame_8.bytes[1],PID.frame_8.bytes[2],PID.frame_8.bytes[3],PID.frame_8.bytes[4],PID.frame_8.bytes[5],PID.frame_8.bytes[6],PID.frame_8.bytes[7]);
        snprintf (string_frame_9, sizeof(string_frame_9), "ID9: '%03X', Data9: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID9,PID.frame_9.bytes[0],PID.frame_9.bytes[1],PID.frame_9.bytes[2],PID.frame_9.bytes[3],PID.frame_9.bytes[4],PID.frame_9.bytes[5],PID.frame_9.bytes[6],PID.frame_9.bytes[7]);
        snprintf (string_frame_10, sizeof(string_frame_10), "ID10: '%03X', Data10: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID10,PID.frame_10.bytes[0],PID.frame_10.bytes[1],PID.frame_10.bytes[2],PID.frame_10.bytes[3],PID.frame_10.bytes[4],PID.frame_10.bytes[5],PID.frame_10.bytes[6],PID.frame_10.bytes[7]);
        snprintf (string_frame_11, sizeof(string_frame_11), "ID11: '%03X', Data11: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID11,PID.frame_11.bytes[0],PID.frame_11.bytes[1],PID.frame_11.bytes[2],PID.frame_11.bytes[3],PID.frame_11.bytes[4],PID.frame_11.bytes[5],PID.frame_11.bytes[6],PID.frame_11.bytes[7]);
        snprintf (string_frame_12, sizeof(string_frame_12), "ID12: '%03X', Data12: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID12,PID.frame_12.bytes[0],PID.frame_12.bytes[1],PID.frame_12.bytes[2],PID.frame_12.bytes[3],PID.frame_12.bytes[4],PID.frame_12.bytes[5],PID.frame_12.bytes[6],PID.frame_12.bytes[7]);
        sprintf(payload, "{ timestamp: %s, latitud: '%0.6f', longitud: '%0.6f' , %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, msg_cnt: '%lu', SD_data: '%d', signal: '%d', num_sats: '%d', valid_gps: '%d'  }",timestamp,data_gps.latitud,data_gps.longitud,string_frame_1,string_frame_2,string_frame_3,string_frame_4,string_frame_5,string_frame_6,string_frame_7,string_frame_8,string_frame_9,string_frame_10,string_frame_11,string_frame_12,msg_count,SD_data,signals,data_gps.num_sats,data_gps.validGps);
#endif 

#ifdef RENUALT_KANGOO_ZE
        static int cnt_volt_cell = 0;
        cnt_volt_cell++;
        snprintf (string_frame_1, sizeof(string_frame_1), "ID1: '%03X', Data1: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID1,PID.frame_1.bytes[0],PID.frame_1.bytes[1],PID.frame_1.bytes[2],PID.frame_1.bytes[3],PID.frame_1.bytes[4],PID.frame_1.bytes[5],PID.frame_1.bytes[6],PID.frame_1.bytes[7]);
        snprintf (string_frame_2, sizeof(string_frame_2), "ID2: '%03X', Data2: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID2,PID.frame_2.bytes[0],PID.frame_2.bytes[1],PID.frame_2.bytes[2],PID.frame_2.bytes[3],PID.frame_2.bytes[4],PID.frame_2.bytes[5],PID.frame_2.bytes[6],PID.frame_2.bytes[7]);
        snprintf (string_frame_3, sizeof(string_frame_3), "ID3: '%03X', Data3: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID3,PID.frame_3.bytes[0],PID.frame_3.bytes[1],PID.frame_3.bytes[2],PID.frame_3.bytes[3],PID.frame_3.bytes[4],PID.frame_3.bytes[5],PID.frame_3.bytes[6],PID.frame_3.bytes[7]);
        snprintf (string_frame_4, sizeof(string_frame_4), "ID4: '%03X', Data4: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID4,PID.frame_4.bytes[0],PID.frame_4.bytes[1],PID.frame_4.bytes[2],PID.frame_4.bytes[3],PID.frame_4.bytes[4],PID.frame_4.bytes[5],PID.frame_4.bytes[6],PID.frame_4.bytes[7]);
        snprintf (string_frame_5, sizeof(string_frame_5), "ID5: '%03X', Data5: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID5,PID.frame_5.bytes[0],PID.frame_5.bytes[1],PID.frame_5.bytes[2],PID.frame_5.bytes[3],PID.frame_5.bytes[4],PID.frame_5.bytes[5],PID.frame_5.bytes[6],PID.frame_5.bytes[7]);
        snprintf (string_frame_6, sizeof(string_frame_6), "ID6: '%03X', Data6: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID6,PID.frame_6.bytes[0],PID.frame_6.bytes[1],PID.frame_6.bytes[2],PID.frame_6.bytes[3],PID.frame_6.bytes[4],PID.frame_6.bytes[5],PID.frame_6.bytes[6],PID.frame_6.bytes[7]);
        snprintf (string_frame_7, sizeof(string_frame_7), "ID7: '%03X', Data7: '%d%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X'",ID7,cnt_volt_cell,PID.groupVolt1_21.bytes[0],PID.groupVolt1_21.bytes[1],PID.groupVolt1_21.bytes[2],PID.groupVolt1_21.bytes[3],PID.groupVolt1_21.bytes[4],PID.groupVolt1_21.bytes[5],PID.groupVolt1_21.bytes[6],PID.groupVolt1_21.bytes[7],PID.groupVolt1_22.bytes[0],PID.groupVolt1_22.bytes[1],PID.groupVolt1_22.bytes[2],PID.groupVolt1_22.bytes[3],PID.groupVolt1_22.bytes[4],PID.groupVolt1_22.bytes[5],PID.groupVolt1_23.bytes[0],PID.groupVolt1_23.bytes[1],PID.groupVolt1_23.bytes[2],PID.groupVolt1_23.bytes[3],PID.groupVolt1_23.bytes[4],PID.groupVolt1_23.bytes[5],PID.groupVolt1_23.bytes[6],PID.groupVolt1_23.bytes[7],PID.groupVolt1_24.bytes[0],PID.groupVolt1_24.bytes[1],PID.groupVolt1_24.bytes[2],PID.groupVolt1_24.bytes[3],PID.groupVolt1_24.bytes[4],PID.groupVolt1_24.bytes[5],PID.groupVolt1_25.bytes[0],PID.groupVolt1_25.bytes[1],PID.groupVolt1_25.bytes[2],PID.groupVolt1_25.bytes[3],PID.groupVolt1_25.bytes[4],PID.groupVolt1_25.bytes[5],PID.groupVolt1_25.bytes[6],PID.groupVolt1_25.bytes[7],PID.groupVolt1_26.bytes[0],PID.groupVolt1_26.bytes[1],PID.groupVolt1_26.bytes[2],PID.groupVolt1_26.bytes[3],PID.groupVolt1_26.bytes[4],PID.groupVolt1_26.bytes[5],PID.groupVolt1_27.bytes[0],PID.groupVolt1_27.bytes[1],PID.groupVolt1_27.bytes[2],PID.groupVolt1_27.bytes[3],PID.groupVolt1_27.bytes[4],PID.groupVolt1_27.bytes[5]);
        snprintf (string_frame_8, sizeof(string_frame_8), "ID8: '%03X', Data8: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID8,PID.frame_8.bytes[0],PID.frame_8.bytes[1],PID.frame_8.bytes[2],PID.frame_8.bytes[3],PID.frame_8.bytes[4],PID.frame_8.bytes[5],PID.frame_8.bytes[6],PID.frame_8.bytes[7]);
        snprintf (string_frame_9, sizeof(string_frame_9), "ID9: '%03X', Data9: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID9,PID.frame_9.bytes[0],PID.frame_9.bytes[1],PID.frame_9.bytes[2],PID.frame_9.bytes[3],PID.frame_9.bytes[4],PID.frame_9.bytes[5],PID.frame_9.bytes[6],PID.frame_9.bytes[7]);
        snprintf (string_frame_10, sizeof(string_frame_10), "ID10: '%03X', Data10: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID10,PID.frame_10.bytes[0],PID.frame_10.bytes[1],PID.frame_10.bytes[2],PID.frame_10.bytes[3],PID.frame_10.bytes[4],PID.frame_10.bytes[5],PID.frame_10.bytes[6],PID.frame_10.bytes[7]);
        if(cnt_volt_cell == 4){
                cnt_volt_cell = 0;
        }
        sprintf(payload, "{ timestamp: %s, latitud: '%0.6f', longitud: '%0.6f', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, msg_cnt: '%lu', SD_data: '%d', signal: '%d', num_sats: '%d', valid_gps: '%d'  }",timestamp,data_gps.latitud,data_gps.longitud,string_frame_1,string_frame_2,string_frame_3,string_frame_4,string_frame_5,string_frame_6,string_frame_7,string_frame_8,string_frame_9,string_frame_10,msg_count,SD_data,signals,data_gps.num_sats,data_gps.validGps);
#endif

#ifdef BUS_YUTONG
        snprintf (string_frame_1, sizeof(string_frame_1), "ID1: '%08X', Data1: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID1,PID.frame_1.bytes[0],PID.frame_1.bytes[1],PID.frame_1.bytes[2],PID.frame_1.bytes[3],PID.frame_1.bytes[4],PID.frame_1.bytes[5],PID.frame_1.bytes[6],PID.frame_1.bytes[7]);
        snprintf (string_frame_2, sizeof(string_frame_2), "ID2: '%08X', Data2: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID2,PID.frame_2.bytes[0],PID.frame_2.bytes[1],PID.frame_2.bytes[2],PID.frame_2.bytes[3],PID.frame_2.bytes[4],PID.frame_2.bytes[5],PID.frame_2.bytes[6],PID.frame_2.bytes[7]);
        snprintf (string_frame_3, sizeof(string_frame_3), "ID3: '%08X', Data3: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID3,PID.frame_3.bytes[0],PID.frame_3.bytes[1],PID.frame_3.bytes[2],PID.frame_3.bytes[3],PID.frame_3.bytes[4],PID.frame_3.bytes[5],PID.frame_3.bytes[6],PID.frame_3.bytes[7]);
        snprintf (string_frame_4, sizeof(string_frame_4), "ID4: '%08X', Data4: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID4,PID.frame_4.bytes[0],PID.frame_4.bytes[1],PID.frame_4.bytes[2],PID.frame_4.bytes[3],PID.frame_4.bytes[4],PID.frame_4.bytes[5],PID.frame_4.bytes[6],PID.frame_4.bytes[7]);
        snprintf (string_frame_5, sizeof(string_frame_5), "ID5: '%08X', Data5: '%02X%02X%02X%02X%02X%02X%02X%02X'",ID5,PID.frame_5.bytes[0],PID.frame_5.bytes[1],PID.frame_5.bytes[2],PID.frame_5.bytes[3],PID.frame_5.bytes[4],PID.frame_5.bytes[5],PID.frame_5.bytes[6],PID.frame_5.bytes[7]);
        sprintf(payload, "{ timestamp: %s, latitud: '%0.6f', longitud: '%0.6f', %s, %s, %s, %s, %s, msg_cnt: '%lu', SD_data: '%d', signal: '%d', num_sats: '%d', valid_gps: '%d'  }",timestamp,data_gps.latitud,data_gps.longitud,string_frame_1,string_frame_2,string_frame_3,string_frame_4,string_frame_5,msg_count,SD_data,signals,data_gps.num_sats,data_gps.validGps);
#endif
        
        strcpy(mqtt_payload, payload);
        //uSD_save_data(payload);
}

#ifdef RENUALT_KANGOO_ZE
void PID_Filter_kangoo(CAN_FRAME incoming, OBD PID){
        switch(incoming.id) {
                case ID1  :
                        PID.frame_1=incoming.data;
                break; 
                case ID2  :
                        PID.frame_2=incoming.data;
                break; 
                case ID3  :
                        PID.frame_3=incoming.data;
                break; 
                case ID4  :
                        PID.frame_4=incoming.data;
                break; 
                case ID5  :
                        PID.frame_5=incoming.data;
                break; 
                case ID6  :
                        PID.frame_6=incoming.data;
                break;
                case ID7  :

                        switch (incoming.data.bytes[0])
                        {
                        case 0x21:

                                PID.groupVolt1_21.bytes[0] = incoming.data.bytes[1];
                                PID.groupVolt1_21.bytes[1] = incoming.data.bytes[2];
                                PID.groupVolt1_21.bytes[2] = incoming.data.bytes[3];
                                PID.groupVolt1_21.bytes[3] = incoming.data.bytes[4];
                                PID.groupVolt1_21.bytes[4] = incoming.data.bytes[5];
                                PID.groupVolt1_21.bytes[5] = incoming.data.bytes[6];
                                PID.groupVolt1_21.bytes[6] = incoming.data.bytes[7];
                        break;

                        case 0x22:

                                PID.groupVolt1_21.bytes[7] = incoming.data.bytes[1];
                                PID.groupVolt1_22.bytes[0] = incoming.data.bytes[2];
                                PID.groupVolt1_22.bytes[1] = incoming.data.bytes[3];
                                PID.groupVolt1_22.bytes[2] = incoming.data.bytes[4];
                                PID.groupVolt1_22.bytes[3] = incoming.data.bytes[5];
                                PID.groupVolt1_22.bytes[4] = incoming.data.bytes[6];
                                PID.groupVolt1_22.bytes[5] = incoming.data.bytes[7];

                        break;
                        

                        case 0x23:

                                PID.groupVolt1_23.bytes[0] = incoming.data.bytes[1];
                                PID.groupVolt1_23.bytes[1] = incoming.data.bytes[2];
                                PID.groupVolt1_23.bytes[2] = incoming.data.bytes[3];
                                PID.groupVolt1_23.bytes[3] = incoming.data.bytes[4];
                                PID.groupVolt1_23.bytes[4] = incoming.data.bytes[5];
                                PID.groupVolt1_23.bytes[5] = incoming.data.bytes[6];
                                PID.groupVolt1_23.bytes[6] = incoming.data.bytes[7];
                        
                        break;

                        case 0x24:

                                PID.groupVolt1_23.bytes[7] = incoming.data.bytes[1];
                                PID.groupVolt1_24.bytes[0] = incoming.data.bytes[2];
                                PID.groupVolt1_24.bytes[1] = incoming.data.bytes[3];
                                PID.groupVolt1_24.bytes[2] = incoming.data.bytes[4];
                                PID.groupVolt1_24.bytes[3] = incoming.data.bytes[5];
                                PID.groupVolt1_24.bytes[4] = incoming.data.bytes[6];
                                PID.groupVolt1_24.bytes[5] = incoming.data.bytes[7];

                        break;

                        case 0x25:
                                PID.groupVolt1_25.bytes[0] = incoming.data.bytes[1];
                                PID.groupVolt1_25.bytes[1] = incoming.data.bytes[2];
                                PID.groupVolt1_25.bytes[2] = incoming.data.bytes[3];
                                PID.groupVolt1_25.bytes[3] = incoming.data.bytes[4];
                                PID.groupVolt1_25.bytes[4] = incoming.data.bytes[5];
                                PID.groupVolt1_25.bytes[5] = incoming.data.bytes[6];
                                PID.groupVolt1_25.bytes[6] = incoming.data.bytes[7];

                        break;

                        case 0x26:
                                PID.groupVolt1_25.bytes[7] = incoming.data.bytes[1];
                                PID.groupVolt1_26.bytes[0] = incoming.data.bytes[2];
                                PID.groupVolt1_26.bytes[1] = incoming.data.bytes[3];
                                PID.groupVolt1_26.bytes[2] = incoming.data.bytes[4];
                                PID.groupVolt1_26.bytes[3] = incoming.data.bytes[5];
                                PID.groupVolt1_26.bytes[4] = incoming.data.bytes[6];
                                PID.groupVolt1_26.bytes[5] = incoming.data.bytes[7];

                        break;
                        case 0x27:
                                PID.groupVolt1_27.bytes[0] = incoming.data.bytes[1];
                                PID.groupVolt1_27.bytes[1] = incoming.data.bytes[2];
                                PID.groupVolt1_27.bytes[2] = incoming.data.bytes[3];
                                PID.groupVolt1_27.bytes[3] = incoming.data.bytes[4];
                                PID.groupVolt1_27.bytes[4] = incoming.data.bytes[5];
                                PID.groupVolt1_27.bytes[5] = incoming.data.bytes[6];

                        break;
                        default:
                        break;
                        }
                break; 
                case ID8  :
                        PID.frame_8=incoming.data;
                break;
                case ID9  :
                        PID.frame_9=incoming.data;
                break;
                case ID10  :
                        PID.frame_10=incoming.data;
                break;
        }
}
#endif

#ifdef BUS_YUTONG
void PID_Filter_yutong(CAN_FRAME incoming, OBD PID){
        switch(incoming.id) {
                case ID1  :
                        PID.frame_1=incoming.data;
                break; 
                case ID2  :
                        PID.frame_2=incoming.data;
                break; 
                case ID3  :
                        PID.frame_3=incoming.data;
                break; 
                case ID4  :
                        PID.frame_4=incoming.data;
                break; 
                case ID5  :
                        PID.frame_5=incoming.data;
                break; 
        }
}
#endif

#ifdef MITSUBISHI_I_MIEV
void PID_Filter_imiev(CAN_FRAME incoming, OBD PID){
        switch(incoming.id) {
                case ID1  :
                        PID.frame_1=incoming.data;
                break; 
                case ID2  :
                        PID.frame_2=incoming.data;
                break; 
                case ID3  :
                        PID.frame_3=incoming.data;
                break; 
                case ID4  :
                        PID.frame_4=incoming.data;
                break; 
                case ID5  :
                        PID.frame_5=incoming.data;
                break; 
                case ID6  :
                        PID.frame_6=incoming.data;
                break;
                case ID7  :
                        PID.frame_7=incoming.data;
                break; 
                case ID8  :
                        PID.frame_8=incoming.data;
                break;
                case ID9  :
                        PID.frame_9=incoming.data;
                break;
                case ID10  :
                        PID.frame_10=incoming.data;
                break; 
                case ID11  :
                        PID.frame_11=incoming.data;
                break; 
                
                case ID12  :
                        PID.frame_12=incoming.data;
                break; 
        }   
}
#endif

void sendData1_can_bus(int code)
{
	CAN_FRAME outgoing;
	outgoing.id = 0x79B;
	outgoing.extended = false;
	outgoing.priority = 0; //0-15 lower is higher priority
	outgoing.length = 8;
	outgoing.data.s0 = 0x2102;
        outgoing.data.byte[2] = 0x42;
	outgoing.data.byte[3] = 0x00;
	outgoing.data.byte[4] = 0x00;
        outgoing.data.byte[5] = 0x00;
        outgoing.data.byte[6] = 0x00;
	outgoing.data.byte[7] = 0x00;
        outgoing.data.byte[8] = 0x00;
	Can0.sendFrame(outgoing);
}
void sendData2_can_bus()
{
	CAN_FRAME outgoing;
	outgoing.id = 0x79B;
	outgoing.extended = false;
	outgoing.priority = 1; //0-15 lower is higher priority
	outgoing.length = 8;
	outgoing.data.s0 = 0x0130;
        outgoing.data.byte[2] = 0x00;
	outgoing.data.byte[3] = 0x00;
	outgoing.data.byte[4] = 0x00;
        outgoing.data.byte[5] = 0x00;
        outgoing.data.byte[6] = 0x00;
	outgoing.data.byte[7] = 0x00;
        outgoing.data.byte[8] = 0x00;
	//outgoing.data.high = 0x000000;
	Can0.sendFrame(outgoing);
}
