#ifndef _MY_TYPE_H_
#define _MY_TYPE_H_ 	   

union U32_DATA
{
	unsigned int u32;
	unsigned short u16[2];
	unsigned char u8[4];
};

union U16_DATA
{
	unsigned short u16;
	unsigned char u8[2];
};

#endif	// _MY_TYPE_H_

