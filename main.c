#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <unistd.h>

#define ADDRESS     "127.0.0.1:1883"
#define CLIENTID    "MQTT_Client"
#define TOPIC       "test_topic"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("接收到消息：%.*s\n", message->payloadlen, (char *)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\n连接断开: %s\n", cause);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    // 创建MQTT客户端
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // 设置连接选项
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // 设置回调函数
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);

    // 连接到MQTT代理
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("连接到MQTT代理失败，错误代码：%d\n", rc);
        exit(EXIT_FAILURE);
    }

    // 发布消息
    pubmsg.payload = "Hello, MQTT!";
    pubmsg.payloadlen = (int)strlen(pubmsg.payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

    // 等待消息传递完成
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("消息发布完成，结果：%d\n", rc);

    // 订阅主题
    MQTTClient_subscribe(client, TOPIC, QOS);

    // 保持循环以接收消息
    while (1) {
        usleep(10000); // 等待10毫秒以允许接收消息
    }

    // 断开连接
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);

    return rc;
}
