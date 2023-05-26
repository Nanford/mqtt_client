#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <pthread.h>
#include <stdbool.h>
#include "util.h"
#include "cJSON.h"
#include "mqtt_client.h"
#include <time.h>

#define ADDRESS     "127.0.0.1:1883"
#define CLIENTID    "daemonProcess"
#define TOPIC       "wuhan/zsk/scada/agv/outbound/pub"
//#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
#define SUB_TOPIC  "longyan/ysk/scada/fxj/action/data"

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

static int CONNECT = 0;

pthread_t mainThreadId, mqttThreadId;

void delivered(void* context, MQTTClient_deliveryToken dt)
{
    // printf("Message with token value %d delivery confirmed\n", dt);
    token = dt;
}

int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message)
{
    //printf(" message:%s\n", message->payload);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void MQTT_Destroy(void)
{
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    pthread_cancel(mqttThreadId);
}

void connlost(void* context, char* cause)
{
    printf("\nConnection lost\n");
    printf(" cause: %s\n", cause);
    MQTTClient_disconnect(client, 10000);
    CONNECT = 0;
}


int publish_data(char* payload, uint32_t length)
{
	int rc;
	pubmsg.payload = payload;
	pubmsg.payloadlen = length;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;

	if (CONNECT) {
		MQTTClient_publishMessage(client, SUB_TOPIC, &pubmsg, &token);
		/*rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
		printf("Message with delivery token %d delivered\n", token);*/
	}
	else
	{
		return -1;
	}
	return 0;
}

void * mqtt_init()
{
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    printf("create MQTT.\n");
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    printf("signup MQTT.\n");
    while (1) {
        if (CONNECT) {
            usleep(500);
        }
        else
        {
            if ((rc = MQTTClient_connect(client, &conn_opts)) != 0)
            {
                printf("Failed to connect, return code %d\n", rc);
                exit(EXIT_FAILURE);
            }
            else
            {
                CONNECT = 1;
                printf("daemon sub_topic %s\n", TOPIC);
                MQTTClient_subscribe(client,TOPIC, QOS);
            }
        }
    }

}


int run_mqtt(void)
{
    if (pthread_create(&mqttThreadId, NULL, mqtt_init, NULL) != 0) {
        printf("Failed to create MQTT thread.\n");
        return 1;
    }
   // pthread_join(mainThreadId, NULL);
	
}

