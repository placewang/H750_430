#ifndef __DELTASERVO_H_
#define __DELTASERVO_H_

#define  UP_NODE_ID    0x02     //上放料节点ID
#define  DOWN_NODE_ID  0x01     //下放料节点ID


/*
标准设备定义对象字典中对象DS402
*/

#define OD_CONTROL_WORD        0x6040 //驱动器控制  bit16
#define OD_STATUS_WORD         0x6041 //驱动器状态  bit16
#define OD_ACC_TIME            0x6083 //加速斜率 ms bit32
#define OD_DEC_TIME            0x6084 //减速斜率 ms bit32
#define OD_SET_TARGET_SPEED    0x60FF //设置目标速度bit32
#define OD_READ_CUR_SPEED      0x606C //读取当前速度bit32
#define OD_SET_MODE            0x6060 //设置模式    bit8
#define OD_SET_TARGET_POSITION 0x607A //设置目标位置bit32
#define OD_SET_SPEED_LIMIT     0x6081 //设置位置模式的限制速度 bit32
#define OD_READ_CUR_POSITION   0x6064 //读取当前位置 bit32


/*
伺服电机运行模式
*/
#define    SERVO_POSITION_MODE 1       //位置模式   
#define    SERVO_SPEED_MODE    3       //速度模式 

extern char appDeltServoType;

void DELTA_Servo_init(void); 
void DELTA_Set_motorID(unsigned char mID);
void DELTA_Servo_GetSpeed(unsigned char ACCnodeid); 
void DELTA_Servo_SetMode(unsigned char node_id,unsigned char mode); 
void DELTA_Servo_SetTargetSpeed(unsigned char,signed int);
void DELTA_Servo_enable(unsigned char,unsigned char);
#endif


