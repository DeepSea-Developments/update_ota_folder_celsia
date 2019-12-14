#ifndef GPRS_H
#define	GPRS_H

#include "vehicle.h"

#define GPRS_BAUDRATE 115200
#define TINY_GSM_MODEM_UBLOX
//#define TINY_GSM_MODEM_SIM5360

#ifdef TINY_GSM_MODEM_UBLOX
  #define PORT 8883
#endif

#ifdef TINY_GSM_MODEM_SIM5360
  #define PORT 1883
#endif

#define TINY_GSM_DEBUG SerialMon
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false  

#define GSM_PIN ""
// set GSM PIN, if any
#define GSM_PIN ""

/*CONNECTION PARAMETERS MQTT**************************************/
// Your GPRS credentials, if any
#define APN "internet.comcel.com.co"
#define USER ""
#define PASSWORD ""
#define BROKER "fleet-qa-ioth.azure-devices.net"

//CREDENCIALES AZURE
/********************************************************/
#ifdef MITSUBISHI_I_MIEV
  #ifdef PLACA1
    #define TOPIC "devices/HDX501/messages/events/"
    #define CLIENT "HDX501"
    #define AZURE_RECV_TOPIC "devices/HDX501/messages/devicebound/#"
    #define USER "fleet-qa-ioth.azure-devices.net/HDX501/?api-version=2018-06-30"
    #define SAS_TOKEN "SharedAccessSignature sr=fleet-qa-ioth.azure-devices.net%2Fdevices%2FHDX501&sig=GL3TkhvhB6BfilQr2RLX3WvGq%2BGZoj8Q43ps6vFOtCY%3D&se=1607629765"
  #endif
  #ifdef PLACA2
    #define TOPIC "devices/HDX148/messages/events/"
    #define CLIENT "HDX148"
    #define AZURE_RECV_TOPIC "devices/HDX148/messages/devicebound/#"
    #define USER "fleet-qa-ioth.azure-devices.net/HDX148/?api-version=2018-06-30"
    #define SAS_TOKEN "SharedAccessSignature sr=fleet-qa-ioth.azure-devices.net%2Fdevices%2FHDX148&sig=n29GZR0%2FNhgr6sCQEVKt0HWhxh1F4p98pCxu0WD15a0%3D&se=1606764306"
  #endif
#endif

#ifdef RENUALT_KANGOO_ZE
  #ifdef PLACA1
    #define TOPIC "devices/FWQ388/messages/events/"
    #define CLIENT "FWQ388"
    #define AZURE_RECV_TOPIC "devices/FWQ388/messages/devicebound/#"
    #define USER "fleet-qa-ioth.azure-devices.net/FWQ388/?api-version=2018-06-30"
    #define SAS_TOKEN "SharedAccessSignature sr=fleet-qa-ioth.azure-devices.net%2Fdevices%2FFWQ388&sig=Na%2FhiTMWrWrBgXaCB6bWkYRFGwMblaVxop6453%2FeShM%3D&se=1607629563"
  #endif
  #ifdef PLACA2
    #define TOPIC "devices/FWQ368/messages/events/"
    #define CLIENT "FWQ368"
    #define AZURE_RECV_TOPIC "devices/FWQ368/messages/devicebound/#"
    #define USER "fleet-qa-ioth.azure-devices.net/FWQ368/?api-version=2018-06-30"
    #define SAS_TOKEN "SharedAccessSignature sr=fleet-qa-ioth.azure-devices.net%2Fdevices%2FFWQ368&sig=llLIec9SOlV3WCn5wNDwCiHLIaBGNeomRD9MDADnbNA%3D&se=1607629712"
  #endif
#endif

#ifdef BUS_YUTONG
  #define TOPIC "devices/FWQ191/messages/events/"
  #define CLIENT "FWQ191"
  #define AZURE_RECV_TOPIC "devices/FWQ191/messages/devicebound/#"
  #define USER "fleet-qa-ioth.azure-devices.net/FWQ191/?api-version=2018-06-30"
  #define SAS_TOKEN "SharedAccessSignature sr=fleet-qa-ioth.azure-devices.net%2Fdevices%2FFWQ191&sig=dCZ%2FVGi7Fdr71C%2F8c4J5cJ1HjfTMUbgofn8uhTrKjaY%3D&se=1607629651"
#endif
/********************************************************/

enum STATUS{
  CONNECTED = 1,
  DISCONNECTED =2,
};

void GPRS_init(void);

void GPRS_run(void);

void GPRS_reset(void);

void GPRS_send_mqtt(char JSONPAYLOAD[1000]);

bool GPRS_connected();

int GPRS_signalStrenght();

bool mqttConnect();

#endif	/* GPRS_H */