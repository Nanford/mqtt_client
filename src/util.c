#include <stdio.h>
#include "util.h"

int arrayToStr(unsigned char* buf, unsigned int buflen, char* out)
{
	unsigned int i, pos = 0;
	for (i = 0; i < buflen; i++)
	{
		sprintf(out + pos, "%02x", buf[i]);

		pos += 2;
	}
	return buflen * 2;
}

/* data convert 4 byte to int*/
uint32_t ConvertTo32(char* buf)
{
	uint32_t temp_value = 0;
	temp_value = (uint32_t)((buf[0] << 24) & 0xFF000000);
	temp_value |= (uint32_t)((buf[1] << 16) & 0xFF0000);
	temp_value |= (uint32_t)((buf[2] << 8) & 0xFF00);
	temp_value |= (uint32_t)(buf[3] & 0xFF);
	//std::cout << "ConvertTo32£º" << std::hex << (uint32_t)temp_value << std::endl;
	return temp_value;
}

/* data convert 2 byte to short*/
uint16_t ConvertTo16(char* buf)
{
	uint16_t temp_value = 0;
	temp_value = (uint16_t)((buf[0] << 8) & 0xFF00);
	temp_value |= (uint16_t)(buf[1] & 0xFF);
	//std::cout << "ConvertTo16£º" << std::hex << (uint16_t)temp_value << std::endl;
	return temp_value;
}