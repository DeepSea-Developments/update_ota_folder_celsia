#ifndef TASK_HANDLER_H
#define	TASK_HANDLER_H


//PROTOTYPES FUNCTIONS
static void can_bus_filter_task(void* arg);
static void gprs_send_task(void* arg);
static void gps_adquisition(void* arg);
void task_handler_init(void);

#endif	/* TASK_HANDLER_H */