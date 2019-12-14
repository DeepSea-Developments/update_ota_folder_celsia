#ifndef OBD_H
#define	OBD_H

#include <due_can.h>
#include "vehicle.h"

//PIDs of interest
/********************************************************/
#ifdef MITSUBISHI_I_MIEV
  #define CAN_BUS_RATE CAN_BPS_500K   //CAN-BUS speed
  #define ID1 0x373
  #define ID2 0x696
  #define ID3 0x6E2
  #define ID4 0x6E3
  #define ID5 0x346
  #define ID6 0x6E1
  #define ID7 0x6E4
  #define ID8 0x101
  #define ID9 0x208
  #define ID10 0x374
  #define ID11 0x412
  #define ID12 0x210
#endif
#ifdef RENUALT_KANGOO_ZE
  #define CAN_BUS_RATE CAN_BPS_500K   //CAN-BUS speed
  #define ID1 0x285
  #define ID2 0x654
  #define ID3 0x090
  #define ID4 0x18A
  #define ID5 0x715
  #define ID6 0x1F9
  #define ID7 0x7BB
  #define ID8 0x42E
  #define ID9 0x42A
  #define ID10 0x638
#endif

#ifdef BUS_YUTONG
  #define CAN_BUS_RATE CAN_BPS_250K   //CAN-BUS speed
  #define ID1 0x18FF0824
  #define ID2 0x18FEC117
  #define ID3 0x18FF30F4
  #define ID4 0x18FF31F4
  #define ID5 0x18FF32F4
#endif
/********************************************************/

//Class: OBD
//A class for controling incomming CAN-BUS data.
class OBD{
public:
  BytesUnion frame_1;
  BytesUnion frame_2;
  BytesUnion frame_3;
  BytesUnion frame_4;
  BytesUnion frame_5;
  BytesUnion frame_6;
  BytesUnion frame_7;
  BytesUnion frame_8;
  BytesUnion frame_9;
  BytesUnion frame_10;
  BytesUnion frame_11;
  BytesUnion frame_12;
  BytesUnion groupVolt1_21;
  BytesUnion groupVolt1_22;
  BytesUnion groupVolt1_23;
  BytesUnion groupVolt1_24;
  BytesUnion groupVolt1_25;
  BytesUnion groupVolt1_26;
  BytesUnion groupVolt1_27;
  BytesUnion groupVolt1_28;
  BytesUnion groupVolt1_29;
  BytesUnion groupVolt1_2A;
  BytesUnion groupVolt1_2B;
  BytesUnion groupVolt1_2C;
};

//PROTOTYPES FUNCTIONS
void OBD_init(void);


#endif	/* OBD_H */