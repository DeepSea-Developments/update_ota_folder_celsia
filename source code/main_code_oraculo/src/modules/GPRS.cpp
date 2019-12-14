#include "../oraculo.h"
#include "GPRS.h"
#include <TinyGsmClient.h>
#include <PubSubClient.h>

TinyGsm modem(SerialAT);
TinyGsmClientSecure client(modem);
PubSubClient mqtt(client);

extern struct shared_no_init_vars_tag shared_no_init_vars;
extern const char * ota_upload_now_message;

void GPRS_init(){         
#ifdef TINY_GSM_MODEM_SIM5360
    pinMode(SPI0_MOSI, OUTPUT);       //SIM5360_RST 
    digitalWrite(SPI0_MOSI, LOW);       //SIM5360_RST
    delay(500);
    digitalWrite(SPI0_MOSI, HIGH);       //SIM5360_RST
#endif

#ifdef TINY_GSM_MODEM_UBLOX
    pinMode(XBEE_RST, OUTPUT);       //XBEE_RST 
    digitalWrite(XBEE_RST, LOW);       //XBEE_RST
    delay(500);
    digitalWrite(XBEE_RST, HIGH);       //XBEE_RST
#endif
    
#if DEBUG 
    SerialMon.println("Initializing ...");
    SerialMon.println("Wait...");
#endif
    SerialAT.begin(GPRS_BAUDRATE);
    delay(3000);
    uint32_t p = millis();
    do{
        watchdogReset();
#ifdef TINY_GSM_MODEM_SIM5360
        if (SerialAT.read() == 'B' ){
            break;
        }
#endif
#ifdef TINY_GSM_MODEM_UBLOX
        if (SerialAT.read() == 'S' ){
            break;
        }
#endif   
    }while((millis() - p < 30000));
    delay(1000);
#if DEBUG
    SerialMon.println("LISTO");
    SerialMon.println("Initializing modem...");
#endif
    modem.init();

#if DEBUG
    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);
#endif

    
    /**********************************************************************/
}

bool mqttConnect() {
#if DEBUG
  SerialMon.print("Connecting to ");
  SerialMon.print(BROKER);
#endif
  // Connect to MQTT Broker
  
  //boolean status = mqtt.connect(CLIENT);
  // Or, if you want to authenticate MQTT:
  boolean status = mqtt.connect(CLIENT, USER, SAS_TOKEN);

  if (status == false) {
#if DEBUG
    SerialMon.println(" fail");
#endif
    return false;
  }
#if DEBUG
  SerialMon.println(" success");
#endif
  mqtt.publish(TOPIC, "Connected");
  mqtt.subscribe(AZURE_RECV_TOPIC);
  return mqtt.connected();
}

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  SerialMon.print("Message arrived [");
  SerialMon.print(topic);
  SerialMon.print("]: ");
  SerialMon.write(payload, len);
  SerialMon.println();
  if(strcmp((char *)payload,(const char *)ota_upload_now_message))
  {
    //mqtt.
    shared_no_init_vars.last_state = 4; //request ota update
    watchdogEnable(100);
    watchdogReset();
    while(1);

  }
}


void GPRS_run(){
    
/*#if DEBUG
    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);
#endif*/
    SerialMon.print("Runing GPRS...");
    modem.gprsConnect(APN, USER, PASSWORD);
#if DEBUG
    SerialMon.print("Waiting for network...");
#endif
    if (!modem.waitForNetwork()) {
#if DEBUG
        SerialMon.println(" fail");
#endif  
        GPRS_reset();
        watchdogReset();
        delay(10000);
        watchdogReset();
    }
#if DEBUG
    SerialMon.println(" success");
    if (modem.isNetworkConnected()) {
        SerialMon.println("Network connected");
    }
    // GPRS connection parameters are usually set after network registration
    SerialMon.print(F("Connecting to "));
    SerialMon.println(APN);
#endif
    
  
#if DEBUG
    if (modem.isGprsConnected()) {
        SerialMon.println("GPRS connected");
    }
#endif
  // MQTT Broker setup
  mqtt.setServer(BROKER, PORT);
  mqtt.setCallback(mqttCallback);
  mqttConnect();
}

void GPRS_reset(){
#if DEBUG
    SerialMon.println("Reseting modem ...");
#endif
#ifdef TINY_GSM_MODEM_SIM5360
        SerialAT.write("AT+CRESET\r");
#endif
#ifdef TINY_GSM_MODEM_UBLOX
        SerialAT.write("AT+CFUN=16\r");
#endif       
}

bool GPRS_connected(){
    static unsigned int unconnected = 0;
    static long lastReconnectAttempt = 0;
    static unsigned int cnt_secs_since_reset = 0;
    static bool reseting = false;
    static bool ok_flag = false;
    
    if(!reseting){
        if (!mqtt.connected()) {
#if DEBUG
            SerialMon.println("=== MQTT NOT CONNECTED ===");
            
#endif  
            unsigned long t = millis();
            if (t - lastReconnectAttempt > 10000L) {
                lastReconnectAttempt = t;
                if (mqttConnect()) {
                    unconnected=0;
                    lastReconnectAttempt = 0;
                }else
                {
                    unconnected++;
                } 
            }
            if(unconnected==5){
                unconnected=0;
                GPRS_reset();
                reseting = true;
            }
            return false;
        }else
        {
            return true;
        } 
    }else{
        cnt_secs_since_reset++;
        if((cnt_secs_since_reset>60) && !ok_flag){
            uint32_t t2 = millis();
            do
            {
                SerialAT.write("AT\r");
                if(SerialAT.read() =='K'){
                    ok_flag = true;
                    break;
                }               
            } while ((millis() - t2 < 10000));
            if(ok_flag){
#if DEBUG
                SerialMon.println("Initializing modem...");
#endif
                modem.init();

#if DEBUG
                String modemInfo = modem.getModemInfo();
                SerialMon.print("Modem Info: ");
                SerialMon.println(modemInfo);
#endif
                cnt_secs_since_reset = 0;                 
            }       
        }else if(cnt_secs_since_reset>60 && ok_flag){
            GPRS_run();
            reseting = false;
            ok_flag = false; 
            return true; 
            
        }
        return false;
    }
}

void GPRS_send_mqtt(char JSONPAYLOAD[1000]){
    
    mqtt.publish(TOPIC,JSONPAYLOAD);
    mqtt.loop();
#if DEBUG
    SerialMon.println("Enviado");
#endif 
}

int GPRS_signalStrenght(){
    return modem.getSignalQuality();
}

