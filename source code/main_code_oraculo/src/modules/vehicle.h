#ifndef VEHICLE_H
#define	VEHICLE_H

//#define RENUALT_KANGOO_ZE
#define MITSUBISHI_I_MIEV
//#define BUS_YUTONG

#ifdef MITSUBISHI_I_MIEV
    //#define PLACA1 "HDX501"
    #define PLACA2 "HDX148"
#endif
#ifdef RENUALT_KANGOO_ZE
    //#define PLACA1 "FWQ388"
    #define PLACA2 "FWQ368"

#endif

#ifdef BUS_YUTONG
    #define PLACA "FWQ191"
#endif

#endif	/* VEHICLE_H */