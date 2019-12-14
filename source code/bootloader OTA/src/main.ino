/*
 * Proyecto: CUSTO OTA macchina m2 (Arduino Due)
 * Autor: jesus velasquez castelblanc - DSD
 * Fecha: 11/12/2019
 * Descripción: .
 * 
 * > buffer serial increased to 1024
 * > tinygsm modification for reset the WDT
 */

#define TINY_GSM_MODEM_UBLOX
#define TINY_GSM_RX_BUFFER 1024
//#define TINY_GSM_DEBUG SerialUSB
#include <stdio.h>
#include "variant.h" 
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "flash_efc.h"
#include "efc.h"

#define CUSTOM_OTA_VERSION "0.0.1"
#define DEBUG 1
#define GPRS_BAUDRATE 115200
#define OTA_UPDATE_MAX_N_TRIES 3

#define SerialMon SerialUSB  //serial port for debugging 
#define SerialAT Serial      //serial port fot GPRS module  


#define BOOT_SIZE		0x10000	
#define FLASH_APP1_ADDR		IFLASH1_ADDR//(0x00080000+BOOT_SIZE)




/*
topic
client
usuario 
contraseña
mqtt.subscribe("devices/HDX148/messages/devicebound/#");


*/
// const char apn[]  = "internet.comcel.com.co";
// const char gprsUser[] = "";
// const char gprsPass[] = "";
//https://raw.githubusercontent.com/DeepSea-Developments/update_ota_folder_celsia/master/firmware.bin

//"/Arto1994/oraculo_celcia_ota/master/firmware.bin";
const int  port = 443;

const char string_test_soft_reset[] = "RUN";

const char* const reset_code_table[] ={
	"hw reset",
	"fw reset",
	"boot failure",
	"app failure",	
	"app_ota_upgrade_request",
  "no_app",
	"unknown"
};
 

const uint32_t * pointer_devices_version_dir = (const uint32_t *)0x000FFF00; 

//const char * data_x = (const char *)(*(pointer_devices_version_dir+4));

const char * pointer_devices_version = (const char *)(*(pointer_devices_version_dir));
const char * pointer_topic = (const char *)(*(pointer_devices_version_dir+1));
const char * pointer_client = (const char *)(*(pointer_devices_version_dir+2));
const char * pointer_azure_recv_topic = (const char *)(*(pointer_devices_version_dir+3));
const char * pointer_user = (const char *)(*(pointer_devices_version_dir+4));
const char * pointer_sas_token = (const char *)(*(pointer_devices_version_dir+5));
const char * pointer_apn = (const char *)(*(pointer_devices_version_dir+6));
const char * pointer_apn_user = (const char *)(*(pointer_devices_version_dir+7));
const char * pointer_apn_password = (const char *)(*(pointer_devices_version_dir+8));
const char * pointer_broker = (const char *)(*(pointer_devices_version_dir+9));
//const char * pointer_broker = (const char *)0x000FFF00+80;

// #define BROKER "fleet-qa-ioth.azure-devices.net"
// #define TOPIC "devices/HDX148/messages/events/"
// #define CLIENT "HDX148"
#define FORMAT_RESOURCE "/DeepSea-Developments/update_ota_folder_celsia/master/%s/firmware.bin"
 const char server[] = "raw.githubusercontent.com";
 char resource[255];
 


// const char * const shared_constants_pointers[10] = {
//   "v0.0.0",
//   TOPIC,
//   CLIENT,
//   AZURE_RECV_TOPIC,
//   USER,
//   SAS_TOKEN,
//   APN,
//  APN_USER,
//  APN_PASSWORD,
//   BROKER, 
// };

const char app_version = 2; 

#define SIZE_OF_RESET_CODE_TABLE (sizeof(reset_code_table)/sizeof(reset_code_table[0]))
TinyGsm modem(SerialAT);
TinyGsmClientSecure client(modem);
HttpClient http(client, server, port);
PubSubClient mqtt(client);


typedef  void (*pFunction)(void);

void execute_user_code(void)
{
  pFunction JumpToApplication;
	uint32_t JumpAddress;
  /* Change the Vector Table to the USER_FLASH_START
  in case the user application uses interrupts */
  SCB->VTOR = (FLASH_APP1_ADDR & 0x1FFFFF80);

  /* SP (stack pointer) register must be set correctly before jump to the
   *  user application: http://www.keil.com/forum/17342/
   */
  __set_MSP(*(uint32_t *) FLASH_APP1_ADDR);

  // Load contents of second word of user flash - the reset handler address
  // in the applications vector table
    JumpAddress = *(__IO uint32_t*) (FLASH_APP1_ADDR + 4);
		JumpToApplication = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) FLASH_APP1_ADDR);
		JumpToApplication();
}
__attribute__((section (".noinit")))
 struct 
{
  char string_test_reset[sizeof(string_test_soft_reset)];
  uint8_t last_state;
  uint8_t n_soft_reset;
  uint8_t current_reset_code; 
}shared_no_init_vars;

const char * shared_network_info = (const char *)0x000FFF00;

// uint8_t last_state __attribute__((section (".noinit")));
// char string_test_reset[sizeof(string_test_soft_reset)] __attribute__((section (".noinit")));
// uint8_t n_soft_reset __attribute__((section (".noinit")));

extern uint32_t _sfixed;

void watchdogSetup(void)
{
  watchdogEnable(10000);
  watchdogReset();
}

uint8_t init_flash_ota_resource(void)
{
  uint32_t ul_rc;

   #if DEBUG 
      SerialMon.println("Flash Initialization");
   #endif

  ul_rc = flash_init(FLASH_ACCESS_MODE_128, 6);
    if (ul_rc != FLASH_RC_OK) {
      #if DEBUG 
      SerialMon.print("error - ");
      SerialMon.println(ul_rc);
      #endif 
      return 1;
           
    }
      #if DEBUG 
      SerialMon.println("ok");
      #endif    
    #if DEBUG 
      SerialMon.println("Flash Unlock");
    #endif
    ul_rc = flash_unlock(IFLASH1_ADDR,
        IFLASH1_ADDR + IFLASH1_SIZE - 1, 0, 0);
    if (ul_rc != FLASH_RC_OK) {
      #if DEBUG 
      SerialMon.print("error - ");
      SerialMon.println(ul_rc);
      #endif 
      return 1;
      
    }
      #if DEBUG 
      SerialMon.println("ok");
      #endif 
  return 0;
}

uint8_t gprs_init(void)
{
  watchdogReset();
  #if DEBUG 
  SerialMon.println("Modem Restart");
  #endif
  //module reset
  pinMode(XBEE_RST, OUTPUT);
  digitalWrite(XBEE_RST, HIGH); 
  delay(500);      
  digitalWrite(XBEE_RST, LOW);
  delay(500);
  digitalWrite(XBEE_RST, HIGH); 
    
  #if DEBUG 
      SerialMon.println("Waiting Start-up...");
      //SerialMon.println("Wait...");
  #endif
      SerialAT.begin(GPRS_BAUDRATE);
      delay(3000);
      uint32_t p = millis();
      do{

        if (SerialAT.read() == 'S' ){
            break;
        }
        watchdogReset();
      }while((millis() - p < 30000));
      delay(1000);
  #if DEBUG
      SerialMon.println("LISTO");
      SerialMon.println("Initializing modem...");
  #endif
      watchdogReset();
      modem.init();
      watchdogReset();

  #if DEBUG
      String modemInfo = modem.getModemInfo();
      SerialMon.print("Modem Info: ");
      SerialMon.println(modemInfo);
  #endif

  return 0;
}

uint8_t gprs_network_connect(void)
{
  // #if DEBUG
  // SerialMon.print("Connect to network...");
  // #endif

  watchdogReset();
  #if DEBUG
  SerialMon.print(F("Connecting to "));
  SerialMon.println(pointer_apn);
  #endif 
  //modem.gprsConnect(apn, gprsUser, gprsPass);

  if (!modem.gprsConnect(pointer_apn, pointer_apn_user, pointer_apn_password)) {
    #if DEBUG
    SerialMon.println(" fail 1");
    #endif 
    watchdogReset();

    #if DEBUG
    SerialMon.println("Waiting...");
    #endif
    if (!modem.waitForNetwork()) {
      #if DEBUG
      SerialMon.println(" fail"); 
      #endif   
      return 1;
    }
    watchdogReset();

    if (!modem.gprsConnect(pointer_apn, pointer_apn_user, pointer_apn_password)) {
    #if DEBUG
    SerialMon.println(" fail 2");
    #endif     
    return 1;
    }
  }
  

  
  // #if DEBUG
  // SerialMon.println(" OK");
  // #endif   

  // if (modem.isNetworkConnected()) {
  //   #if DEBUG
  //   SerialMon.println("Network connected");
  //   #endif 
  //   //return 0;
  // }

  // #if DEBUG
  // SerialMon.print(F("Connecting to "));
  // SerialMon.print(apn);
  // #endif 
  // if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
  //   #if DEBUG
  //   SerialMon.println(" fail");
  //   #endif     
  //   return 1;
  // }
  // SerialMon.println(" OK");

  return 0;

}

uint8_t ota_upgrade_firmware(void)
{

  uint32_t ul_rc;

  #if DEBUG
   SerialMon.print(F("HTTPS GET request to "));
   SerialMon.print(F("server : "));
   SerialMon.println(server);
   SerialMon.print(F("resource : "));
   SerialMon.println(resource);
   SerialMon.print(F("port : "));
   SerialMon.println(port);
  #endif 
 
  watchdogReset();
  http.connectionKeepAlive(); // Currently, this is needed for HTTPS 
  int err = http.get(resource);
  watchdogReset();
  if (err != 0) {
    #if DEBUG
    SerialMon.print(F("error - ")); 
    SerialMon.println(err);  
    #endif   
    return 1;
  }

  int status = http.responseStatusCode();
  SerialMon.print(F("Response status code: "));
  SerialMon.println(status);
  if (!status) {
    #if DEBUG
    SerialMon.print(F("error - ")); 
    SerialMon.println(status);     
    #endif 
    return 1;
  }
 watchdogReset();
  SerialMon.println(F("Response Headers:"));
  while (http.headerAvailable()) {
    #if DEBUG
    String headerName = http.readHeaderName();
    String headerValue = http.readHeaderValue();
    SerialMon.println("    " + headerName + " : " + headerValue);
    #endif
  }
  #if DEBUG
  int length = http.contentLength();
  if (length >= 0) {
    SerialMon.print(F("Content length is: "));
    SerialMon.println(length);
  }
  if (http.isResponseChunked()) {
    SerialMon.println(F("The response is chunked"));
  }
  #endif
  
  uint8_t chunk_sav[256];
  uint32_t chunk_index = 0;
  while(!http.endOfBodyReached())
  {
    if(http.available())
    {
      int ret = http.read(chunk_sav,256);
      #if DEBUG
      // SerialMon.print(F("chunk_index: "));
      // SerialMon.println(chunk_index);
      // SerialMon.print(F("chunk_size: "));
      // SerialMon.println(ret);
      #endif
      ul_rc = flash_write(FLASH_APP1_ADDR+chunk_index, chunk_sav,	ret, 1);
      if (ul_rc != FLASH_RC_OK) {
      #if DEBUG
      SerialMon.println("chunk write error");
      SerialMon.println(ul_rc);
      #endif
      return 1;
      }
      else
      {
        chunk_index+=ret;
      }      
    }
  }

  #if DEBUG  
  SerialMon.print(F("file size: "));
  SerialMon.println(chunk_index);
  #endif
  uint32_t *pmem_stored = (uint32_t *) FLASH_APP1_ADDR;
  uint8_t *byte_file ;   
  for(uint32_t i=0;i<((chunk_index/4)+1);i++)
  {
    watchdogReset();
  
    if((i<100)||(i>((chunk_index/4)-100)))
    {
      SerialUSB.print("data raw: ");
      SerialUSB.print(i*4);
      SerialUSB.print(" - ");
      byte_file = (uint8_t *)&pmem_stored[i];
      SerialUSB.print(byte_file[0],HEX);      
      SerialUSB.print(byte_file[1],HEX);
      SerialUSB.print(" ");
      SerialUSB.print(byte_file[2],HEX);     
      SerialUSB.println(byte_file[3],HEX);
      
    }
    
  }

return 0;
}

bool mqttConnect() {
#if DEBUG
  SerialMon.print("Connecting to ");
  SerialMon.println(pointer_broker);
#endif
  // Connect to MQTT Broker
  
  //boolean status = mqtt.connect(CLIENT);
  // Or, if you want to authenticate MQTT:
  boolean status = mqtt.connect(pointer_client, pointer_user, pointer_sas_token);
  //("HDX148", "fleet-qa-ioth.azure-devices.net/HDX148/?api-version=2018-06-30", "SharedAccessSignature sr=fleet-qa-ioth.azure-devices.net%2Fdevices%2FHDX148&sig=n29GZR0%2FNhgr6sCQEVKt0HWhxh1F4p98pCxu0WD15a0%3D&se=1606764306");

  if (status == false) {
#if DEBUG
    SerialMon.println(" fail");
#endif
    return false;
  }
#if DEBUG
  SerialMon.println(" success");
#endif 
  mqtt.subscribe(pointer_azure_recv_topic);
  return mqtt.connected();
}

bool msg_answered = false;
const char ota_confirmed_message[] = "{ remot_cmd : 'yes' }";
void mqttCallback(char* topic, byte* payload, unsigned int len) {
  #if DEBUG
  SerialMon.print("Message arrived [");
  SerialMon.print(topic);
  SerialMon.print("]: ");
  SerialMon.write(payload, len);
  SerialMon.println();
  #endif 
  if(memcmp(payload,ota_confirmed_message,sizeof(ota_confirmed_message)))
  {
    msg_answered = true;
  }
  
}

const char * ota_no_app_message = "{ state_app : 'no_app' , request : 'upload_confimation' }";
const char * ota_app_error_message = "{ state_app : 'app_error' , request : 'upload_confimation' }";
const char * ota_updating_message = "{ state_app : 'updating' }";
/*******************************************************************
 ******************************************************************/
void setup() {

#if DEBUG 
delay(3000);
#endif


sprintf(resource,FORMAT_RESOURCE,pointer_client);

  //reset recognition routine

  uint8_t reset_code = 0;

  if(!memcmp(shared_no_init_vars.string_test_reset,string_test_soft_reset,sizeof(string_test_soft_reset))) //soft reset
  {
    shared_no_init_vars.n_soft_reset++;
    reset_code = shared_no_init_vars.last_state; // recover last state
  }
  else //hw reset 
  {
    memcpy(shared_no_init_vars.string_test_reset,string_test_soft_reset,sizeof(string_test_soft_reset));
    shared_no_init_vars.n_soft_reset = 0;
  }
  
  shared_no_init_vars.last_state = 2; //executing bootloader
  //valid the code
  if(reset_code > (SIZE_OF_RESET_CODE_TABLE-1))
  {
    reset_code = (SIZE_OF_RESET_CODE_TABLE-1); // unknown
  }
  
  shared_no_init_vars.current_reset_code =  reset_code;
  //bootloader indication
  pinMode(LED_BUILTIN, OUTPUT);
  
   for(uint8_t ii=0;ii<5;ii++)
  {    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  } 

  
  #if DEBUG 
    uint32_t adrss = (uint32_t )&_sfixed;
    SystemCoreClockUpdate();

    SerialMon.begin(115200);
    delay(3000);
    // while (!SerialMon)
    // {
    //   watchdogReset();
    // }
    SerialMon.print("OTA version : ");
    SerialMon.println(CUSTOM_OTA_VERSION);
    SerialMon.print("Clock Speed : ");
    SerialMon.println(SystemCoreClock);
    SerialMon.print("Boot Address : ");
    SerialMon.println(adrss,HEX);
    SerialMon.print("Boot Size : ");
    SerialMon.println(BOOT_SIZE,HEX);
    SerialMon.print("App Address : ");
    SerialMon.println(FLASH_APP1_ADDR,HEX);
    SerialMon.print("RAM no init address : ");
    SerialMon.println((uint32_t)&shared_no_init_vars,HEX);//SerialMon.println((uint32_t)&string_test_reset[0]);
    SerialMon.print("Reset Reason : ");
    SerialMon.println(reset_code_table[reset_code]);
    SerialMon.print("Soft Reset Counter : ");
    SerialMon.println(shared_no_init_vars.n_soft_reset);
    SerialMon.print("Device Version : ");
    SerialMon.println(pointer_devices_version);
    SerialMon.print("Address Device Version : ");
    SerialMon.println((uint32_t)pointer_devices_version,HEX);
    SerialMon.print("Dir Device Version : ");
    SerialMon.println((uint32_t)*pointer_devices_version_dir,HEX);
    SerialMon.print("Data pointer_topic : ");
    SerialMon.println(pointer_topic);
    SerialMon.print("Data pointer_client : ");
    SerialMon.println(pointer_client);
    

    
  #endif

//modem serial initalization
SerialAT.begin(115200);

watchdogReset();
if(init_flash_ota_resource())
{
  //fatal error ,ToDO 
  while(1);
}
uint8_t intentos_modem_connect = 3;
uint8_t intentos_modem_reset = 2;
while(((!modem.isNetworkConnected())&&intentos_modem_reset)||(intentos_modem_reset == 2))
{
  intentos_modem_reset--;
  (void)gprs_init();
  delay(3000);
  intentos_modem_connect = 3;
  while(gprs_network_connect() && intentos_modem_connect)
  {
    intentos_modem_connect--;
    watchdogReset();
    delay(1000);
  }
  delay(1000);
}

//aqui ya hay conexion al APN
delay(500);
if(modem.isNetworkConnected())
{
  
  #if DEBUG 
    SerialMon.println("Network Connected!");
  #endif
  intentos_modem_reset = 1;
  delay(3000);
}
else
{
 intentos_modem_reset = 0;
}

if(intentos_modem_reset)
{
mqtt.setServer(pointer_broker, 8883);
mqtt.setCallback(mqttCallback);
}
uint8_t tries_mqtt = 3;
uint8_t triggered_msg_sent = 0;

uint32_t  timeout_msg_start = 0;
bool no_app_flag = false;
bool app_error_flag = false;
bool only_advise_flag = false;
bool confirmed = false;

	uint32_t app_start_address;
// "hw reset",
// 	"fw reset",
// 	"boot failure",
// 	"app failure",	
// 	"app_ota_upgrade_request",
// 	"unknown"
if(shared_no_init_vars.last_state == 4 )
{
  only_advise_flag = true;
}
else if((shared_no_init_vars.n_soft_reset > 3)&&(reset_code == 3 ))
{
  app_error_flag = true;
  no_app_flag = false;
}
else
{
  only_advise_flag = true;
}

/* Load the Reset Handler address of the application */
app_start_address = *(uint32_t *)(FLASH_APP1_ADDR + 4);

/**
 * Test reset vector of application @APP_START_ADDRESS+4
 * Stay in SAM-BA if *(APP_START+0x4) == 0xFFFFFFFF
 * Application erased condition
 */
if (app_start_address == 0xFFFFFFFF) {
  /* Stay in Bootloader */
  app_error_flag = false;
  no_app_flag = true;
  only_advise_flag = false;
}

  #if DEBUG 
          SerialMon.print(F("app_error_flag : "));
          SerialMon.println(app_error_flag);
          SerialMon.print(F("no_app_flag : "));
          SerialMon.println(no_app_flag);
          SerialMon.print(F("only_advise_flag : "));
          SerialMon.println(only_advise_flag);
  #endif
///*
while(tries_mqtt && intentos_modem_reset)
{
  if (!mqtt.connected())
  { 
    mqttConnect();
    watchdogReset();
    char buff[400];

    sprintf(buff,"{ Reset_Reason : '%s' , Soft_Reset_Counter : '%u', Booloader_version : '%s' , Device_version : '%s' }",reset_code_table[reset_code],shared_no_init_vars.n_soft_reset,CUSTOM_OTA_VERSION,pointer_devices_version);
    mqtt.publish(pointer_topic, buff);    
    delay(2000);
    tries_mqtt--;
  }
  else
  {
    
    if (only_advise_flag)
    {
       break;       
    }
    else if(!triggered_msg_sent)
    {
      triggered_msg_sent = 1;
      if(no_app_flag)
      mqtt.publish(pointer_topic,ota_no_app_message);
      if(app_error_flag)
      mqtt.publish(pointer_topic,ota_app_error_message);
      timeout_msg_start = millis();
      msg_answered = false;     
    }
    else
    {
      if(msg_answered)
      {
          #if DEBUG 
          SerialMon.println(F("from Azure - Confirmed"));
          #endif
          confirmed = true;
          break;
      }
      else
      {
        if((millis()-timeout_msg_start)>5000)
        {
          break;
        }
      }
      
      

    }
    
  }
  watchdogReset();
  mqtt.loop();
}
if(only_advise_flag && intentos_modem_reset)
{
  timeout_msg_start = millis();
  while((millis()-timeout_msg_start)<3000)
  {
    watchdogReset();
    mqtt.loop();
  }
}
if(intentos_modem_reset)
{
mqtt.disconnect();
mqtt.loop();
}
//*/

if(intentos_modem_reset)
{
  if((confirmed)||(no_app_flag)||(reset_code == 4))
  {
    uint8_t ota_update_tries = OTA_UPDATE_MAX_N_TRIES;
    while(ota_upgrade_firmware() && ota_update_tries)
    {
      watchdogReset();
      ota_update_tries--;
      delay(1000);
    }
    if(ota_update_tries)
    {
      shared_no_init_vars.n_soft_reset = 0;
      no_app_flag = false;
    }
  }
}

watchdogReset();
if(intentos_modem_reset)http.stop();
#if DEBUG 
SerialMon.println(F("Server disconnected"));
#endif

modem.gprsDisconnect();
#if DEBUG 
SerialMon.println(F("GPRS disconnected"));
#endif
watchdogReset();

 

if(no_app_flag)
{
  shared_no_init_vars.last_state = 5;
}
else
{
  #if DEBUG 
  SerialMon.println(F("Jumping to APP!"));
  delay(500); 
  USBDevice.detach();
  delay(500);
  #endif 
  shared_no_init_vars.last_state = 3; //executing app   
  execute_user_code();
}
watchdogEnable(1000);
watchdogReset();
while(1);
  
}

void loop() {
 
}

