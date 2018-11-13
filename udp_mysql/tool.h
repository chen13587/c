#ifndef UDP_MYSQL_TOOL_H
#define UDP_MYSQL_TOOL_H
#include <stdio.h>
#include "iostream"
#include "stdint.h"
uint16_t get_crc16(uint8_t* pack, uint16_t length);

uint16_t u8_2_u16(uint8_t* dat);
char half_2_u8(uint8_t* dat);
bool check_crc(uint8_t* buf,uint16_t buf_len,uint8_t* dst);


#endif
