#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cJSON.h>
#include <mqtt_client.h>
#include "util.h"
#include <time.h>

uint32_t sleep_time_ms = 0;

//open file read return char buffer
char* read_file(const char* filename)
{
	char* buf;
	FILE* fp;

	if (filename == NULL)
	{
		printf(" %s file not found!", filename);
		return NULL;
	}
	//以追加读写的方式打开文件
	fp = fopen(filename, "r+");
	if (!fp) {
		printf("open file %s failed!", filename);
	}
	//统计文件大小
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	//若使用fseek，ftell统计文件大小，那么在获得大小后要将指针移回原来的位置，因为fseek会改变指针的位置
	fseek(fp, 0, 0);
	buf = (char*)malloc(sizeof(char)*len + 1);
	//读取文件
	fread(buf, len, 1, fp);
	//关闭
	fclose(fp);

	return buf;
}

uint32_t Sleep_time_ms(cJSON* root_json, cJSON* root_json_rear)
{
	cJSON* value_json = NULL;
	cJSON* value_json_rear = NULL;
	char value_string[14] = {0};
	char value_string_rear[14] = {0};
	uint64_t time = 0;
	uint64_t time_rear = 0;
	uint16_t return_time = 0;

	value_json = cJSON_GetObjectItem(root_json, "timestamp");
	if (value_json->type == cJSON_String)
	{
		strcpy(value_string , value_json->valuestring);
	}
	value_json_rear = cJSON_GetObjectItem(root_json_rear, "timestamp");
	if (value_json_rear->type == cJSON_String)
	{
		strcpy(value_string_rear, value_json_rear->valuestring);
	}

	//printf("--->>time is %s,timerear is %s\n", value_string, value_string_rear);
	time = atoi(value_string);
	time_rear = atoi(value_string_rear);
	//printf("*****>> time is %llu ; time_rear is %llu\n", time, time_rear);
	if (((time_rear - time) > 0) && ((time_rear - time)<2500))
	{
		return_time = time_rear - time;
		
	}
	else
	{
		return_time = sleep_time_ms;
	}
	//printf("return time is :%d\n", return_time);
	return return_time;
}



/*config file read function*/
int analise_and_send(char* Filepath)
{

	cJSON* root_json = NULL;
	cJSON* array_json = NULL;
	cJSON* value_json = NULL;
	cJSON* value_json_pre = NULL;
	uint32_t array_size = 0;
	char* json_string = NULL;
	char* value_string = NULL;
	uint32_t i = 0;

    const char* filename = Filepath;
	FILE* file = fopen(filename, "r"); // 打开文件
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }
	
	char buffer[4096]; // 缓冲区大小，用于逐行读取 JSON 内容
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // 处理读取到的一行 JSON 内容
        cJSON* json = cJSON_Parse(buffer);
        if (json != NULL) {
		//printf("************** read json file***************\n");
		//array_json = cJSON_GetObjectItem(json, "Value_array");
		
		value_string = cJSON_PrintUnformatted(json);
		publish_data(value_string,strlen(value_string));
		//printf("********* 213782137821 ***************\n");
        cJSON_Delete(json); // 释放 cJSON 对象
        }
    }

    fclose(file); // 关闭文件




	return 0;
}


int main(int argc, char *argv[]) {

	printf("************** service run ***************\n");
	run_mqtt();
	printf("************** MQTT init ***************\n");
	while (1) {
		//analise_and_send("../../file/zklh_history.json");
		analise_and_send("./zklh_history.json");
		usleep(5000);
		
	}
	//socket_destroy();
	MQTT_Destroy();
	return 0;
}
