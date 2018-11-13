#include "stdio.h"
#include "iostream"
#include "tool.h"
#include <stdbool.h>

//#define unsigned char uint8_t
//#define unsigned int  uint16_t

uint16_t u8_2_u16(uint8_t* dat){			//拼接2个byte为word
    return (int)dat[0]<<8|(int)dat[1];
}

char half_2_u8(uint8_t* dat){				//拼接2个半字为byte
    return (char)dat[0]<<4|(char)dat[1];
}

uint16_t get_crc16(uint8_t* pack, uint16_t length) {
    uint16_t crc_reg = 0xFFFF;
    uint16_t multi = 0xa001;
    //	uint16_t tmp = 0;
    int i, t;
    for (i = 0; i < length; i++) {
        crc_reg ^= ((uint16_t)pack[i]);
        for (t = 0; t < 8; t++) {						//重复右移8次
            ((crc_reg & 0x01) == 0x01) ? (crc_reg = (crc_reg >> 1) ^ multi) : (crc_reg >>= 1);
        }
    }
    //	tmp = (crc_reg >> 8) & 0x00ff;
    //	return (crc_reg << 8) | tmp;		//结果 颠倒高8低8
    return crc_reg;
}

int hex2ascii(char* in,char* out,int len){
    printf("转换前数据：%s\n", in);
    for(int i=0;i<len*2;i+=2){
        out[i/2]=((((in[i]-0x30)<<4)&0xf0)|(in[i+1]>=0x41?((in[i+1]-0x37)&0x0f):((in[i+1]-0x30)&0x0f)));
    }
    return len/2;
}
bool check_crc(uint8_t* buf,uint16_t buf_len,uint8_t* dst){
    if(u8_2_u16(dst)==get_crc16(buf,buf_len)) {
        return true;
    }
    else{
        printf("CRC error Receive= %x , Correct= %x  \n",u8_2_u16(dst),get_crc16(buf,buf_len));
    }
    return false;
}



