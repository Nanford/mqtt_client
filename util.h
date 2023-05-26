#ifndef UTIL__H
#define UTIL__H

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;




typedef struct {
	char soc_serveraddr[21];
	int soc_port;
	char mqtt_server_url[21];
	int qos;
	int mqtt_port;
	int keep_alive;
	char username[50];
	char password[50];
	char sub_topic[128];
	char pub_topic[128];
}tcpip_communication_t;


uint32_t ConvertTo32(char* buf);
uint16_t ConvertTo16(char* buf);
void cmdProcess(void);


#endif /* #define UTIL__H
 */