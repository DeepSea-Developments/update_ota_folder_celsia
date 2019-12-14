/*
 * Proyecto: Oraculo
 * Autor: DSD
 * Fecha: 8/25/2019
 * Descripción: Adquisición, filtrado y envío de data OBD a servidores de Celsia.
 */
#include "oraculo.h"

#define APN_USER ""
#define APN_PASSWORD ""


 __attribute__((section (".noinit")))
shared_no_init_vars_tag shared_no_init_vars;


//__attribute__((section (".myStore")))
__attribute__((__section__(".mysection")))
const char * const shared_constants_pointers[10] = {
  "v0.0.0",
  TOPIC,
  CLIENT,
  AZURE_RECV_TOPIC,
  USER,
  SAS_TOKEN,
  APN,
  APN_USER,
  APN_PASSWORD,
  BROKER 
};
const char * ota_upload_now_message = "{ remot_cmd : 'upload_now' }";

const char* const reset_code_table[] ={
	"hw reset",
	"fw reset",
	"boot failure",
	"app failure",	
	"app_ota_upgrade_request",
  "no_app",
	"unknown"
};

void setup() {
  watchdogReset();
  delay(2000); //give some time for everything to come up (USB takes time)
  watchdogReset();
  delay(2000); //give some time for everything to come up (USB takes time)
  watchdogReset();
  delay(2000); //give some time for everything to come up (USB takes time)
  
  SerialMon.begin(115200);

    watchdogReset();
  uint8_t reset_code = shared_no_init_vars.current_reset_code;

  if(reset_code>=(sizeof(reset_code_table)/sizeof(reset_code_table[0])))
  {
    reset_code = (sizeof(reset_code_table)/sizeof(reset_code_table[0]))-1;
  }
#if DEBUG
SerialMon.print("current_reset_code: ");
  SerialMon.println(reset_code);
  SerialMon.print("Reset reason: ");
  SerialMon.println(reset_code_table[reset_code]);
  SerialMon.print("Address shared_no_init_vars: ");
  SerialMon.println((uint32_t)&shared_no_init_vars,HEX);
  SerialMon.print("Address device version: ");
  SerialMon.println((uint32_t)&shared_constants_pointers[0],HEX);
  SerialMon.println((uint32_t)&shared_constants_pointers[1],HEX);
  SerialMon.println((uint32_t)&shared_constants_pointers[2],HEX);
    SerialMon.println((uint32_t)&shared_constants_pointers[3],HEX);
  SerialMon.print("String device version: ");
  SerialMon.println(shared_constants_pointers[0]);
  SerialMon.print("string reset control : ");
  if(!shared_no_init_vars.string_test_reset[3])
  {
    SerialMon.println(shared_no_init_vars.string_test_reset);
  }
    
  else
  {
    SerialMon.println("no reset pattern");
  }
  
#endif

  
  
  //*Turning on LEDS and XBee Power Pin
/**********************************************************************/
  pinMode(LED_BUILTIN, OUTPUT);  // DIGI Associate PIN
  pinMode(DS6, OUTPUT);            // Green LED
  pinMode(DS2, OUTPUT);            // Red LED 
  digitalWrite(DS2, LOW); 
/**********************************************************************/


uSD_events_init(); //usD_event_init
GPS_init();   //GPS initialization
RTC_init();
OBD_init();   //CAN-BUS initialization
GPRS_init();  //modem initialization
GPRS_run();   //network and broker connection
task_handler_init();  //create task and run it
  
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
 watchdogReset();
}

