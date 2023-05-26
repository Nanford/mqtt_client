#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__
#define MQTT_CLIENT
#ifdef  MQTT_CLIENT
#include "util.h"


int run_mqtt(void);
void * mqtt_init();
int publish_data(char* payload, uint32_t length);
void MQTT_Destroy(void);
#endif

#endif /*__MQTT_CLIENT_H__*/