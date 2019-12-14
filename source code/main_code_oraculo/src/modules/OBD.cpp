#include "../oraculo.h"
#include "OBD.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
/* Function: OBD_init

   Funcion que inicializa la comunicacion CAN-BUS con el vehículo.

   Parameters:

      CAN_BUS_RATE - Constante que indica la velocidad de comunicacion.

*/
void OBD_init(void){
    /*CAN-BUS SETUP*******************************************************************/
#if DEBUG
    SerialMon.println("Doing Auto Baud scan on CAN1");
#endif
    Can0.begin(CAN_BUS_RATE);
    //By default there are 7 mailboxes for each device that are RX boxes
    //This sets each mailbox to have an open filter that will accept extended
    //or standard frames
    int filterCAN;
    //extended
    for (filterCAN = 0; filterCAN < 3; filterCAN++) {
        Can0.setRXFilter(filterCAN, 0, 0, true);
    }  
    //standard
    for (filterCAN = 3; filterCAN < 7; filterCAN++) {
        Can0.setRXFilter(filterCAN, 0, 0, false);
    }  
    /**********************************************************************/
}