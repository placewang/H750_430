#ifndef _APP_MOTOR_H_
#define _APP_MOTOR_H_

#define MOTOR_SEND_CAN_ID					0x0001		// 闭环电机发送的CAN ID


// 恒强闭环电机的命令字节定义
#define CMD_HQ_MOTOR_FIND_ZERO_BY_HIT		0x01		// 撞零(电机撞停时设置零位)
#define CMD_HQ_MOTOR_SET_PARAMETER			0x02		// 设置基本参数

#define CMD_HQ_MOTOR_YARN_FORWARD_FOLLOW	0x03		// 纱嘴正向跟随
#define CMD_HQ_MOTOR_YARN_REVERSE_FOLLOW	0x04		// 纱嘴反向跟随
#define CMD_HQ_MOTOR_YARN_STOP_FOLLOW		0x05		// 纱嘴停止跟随
#define CMD_HQ_MOTOR_YARN_MOVE				0x06		// 纱嘴移动到

#define CMD_HQ_MOTOR_ENABLE					0x07		// 电机使能
#define CMD_HQ_MOTOR_MOVE_P_WITHOUT_ACK		0x0A		// 位置移动
#define CMD_HQ_MOTOR_MOVE_S_WITHOUT_ACK		0x0B		// 速度移动
#define CMD_HQ_MOTOR_MOVE_STOP_NOW			0x11		// 电机运动停止，并锁定位置
#define CMD_HQ_MOTOR_MOVE_P_WITH_ACK		0x8A		// 带到位回复的位置移动
#define CMD_HQ_MOTOR_GET_STATUS				0x71		// 获取电机状态
#define CMD_HQ_MOTOR_READ_PARAMETER			0x72		// 获取电机数据
#define CMD_HQ_MOTOR_SET_ARRIVAL_IND		0x78		// 设置到位通知，目前用到找零到位通知，发送找零后，再发送
#define CMD_HQ_MOTOR_RESET_ARRIVAL_IND		0x78		// 复位的到位通知，需要先设置
#define CMD_HQ_MOTOR_MOVE_ARRIVAL_IND		0x15		// 移动的到位通知
#define CMD_HQ_MOTOR_FIND_ZERO_BY_SENSOR	0x31		// 归零(传感器方式)

#define CMD_HQ_MOTOR_YARN_SET_ID_MODE		0x50		// 进入和退出设置ID模式
#define CMD_HQ_MOTOR_YARN_GET_DID			0x51		// 读取设备随机标识数
#define CMD_HQ_MOTOR_YARN_SET_LED_MODE		0x52		// 选中并快闪灯（500ms）
#define CMD_HQ_MOTOR_YARN_SET_MOTOR_ID		0x53		// 设置电机新Motor_ID

#define CMD_HQ_MOTOR_YARN_GET_VERSION		0xE7		// 获取电机的版本号


// 电机设置参数类型
#define MOTOR_PARAMETER_TYPE_SET_CODE			0x8001		// 设置编码器值 
#define MOTOR_PARAMETER_TYPE_SET_DIR			0x8002		// 设置方向
#define MOTOR_PARAMETER_TYPE_SET_OFFSET			0x8003		// 设置撞零后移动的偏移量
#define MOTOR_PARAMETER_TYPE_SET_CODE_RATIO		0x8005		// 设置编码器输出比例
#define MOTOR_PARAMETER_TYPE_SET_OFFSET_RATIO	0x8007		// 设置偏移量比例
#define MOTOR_PARAMETER_TYPE_SET_POSITION		0x8008		// 设置电机位置
#define MOTOR_PARAMETER_TYPE_FIND_ZERO_TYPE		0x8009		// 设置找零方式
#define MOTOR_PARAMETER_TYPE_MAX_SPEED			0x800B		// 设置电机最高限速
#define MOTOR_PARAMETER_TYPE_SET_ZERO_REPORT	0x8025		// 设置零位传感器状态自动上报


// 设置参数的消息结构体
struct LOOP_MOTOR_ACK_GET_STATUS
{
	unsigned char cmd;						// 命令
	unsigned char id;						// ID
	unsigned char error;					// 错误常态
	unsigned char signal;					// 信号状态(耙子0，1位表示红外信号（1表示通 0表示不通），2位表示空耙信号（1表示空耙 0表示正常耙）)
	int position;							// 当前位置
}__attribute__((packed));


// 电机参数联合体
union LOOP_MOTOR_PARAMETER
{
	int data_32;
	unsigned int udata_32;
	short data_16[2];
	unsigned short udata_16[2];
	char data_8[4];
	unsigned char udata_8[4];
}__attribute__((packed));

// 设置参数的消息结构体
struct LOOP_MOTOR_MSG_SET_PARAMETER
{
	unsigned char cmd;						// 命令
	unsigned char id;						// ID
	unsigned short parameter_type;			// 参数类型
	union LOOP_MOTOR_PARAMETER parameter;	// 配置的参数
}__attribute__((packed));

// 电机归零
struct LOOP_MOTOR_MSG_FIND_ZERO
{
	unsigned char cmd;			// 命令
	unsigned char id;			// ID
	short speed;				// 速度：最高限速值的百分比（-100到100 正负代表方向）
	unsigned short torque;		// 扭矩的千分比
}__attribute__((packed));


// 电机移动的消息结构，位置移动
struct LOOP_MOTOR_MSG_MOVE_P
{
	unsigned char cmd;			// 命令
	unsigned char id;			// ID
	unsigned short speed;		// 速度：圈/分
	unsigned int position;		// 移动的目标位置
}__attribute__((packed));

// 电机移动的消息结构，速度移动
struct LOOP_MOTOR_MSG_MOVE_S
{
	unsigned char cmd;			// 命令
	unsigned char id;			// ID
	unsigned short torque;		// 扭矩的千分比
	int speed;					// 移动的速度
}__attribute__((packed));

// 电机状态回复的消息结构
struct LOOP_MOTOR_MSG_STATUS_ACK
{
	unsigned char cmd;			// 命令
	unsigned char id;			// ID
	unsigned char error;		// 错误常态
	unsigned char signal;		// 信号状态(耙子0，1位表示红外信号（1表示通 0表示不通），2位表示空耙信号（1表示空耙 0表示正常耙）)
	unsigned int position;		// 当前位置
}__attribute__((packed));

// 电机设置ID的消息结构
struct LOOP_MOTOR_MSG_SET_ID
{
	unsigned char cmd;			// 命令
	unsigned char id;			// ID
	unsigned char mode;			// 模式
	unsigned char data;			// 数据
	unsigned int did;			// 设备ID
}__attribute__((packed));


// 电机获取版本的消息结构
struct LOOP_MOTOR_MSG_GET_VERSION
{
	unsigned char cmd;			// 命令
	unsigned char id;			// ID
	unsigned short device_type;	// 电机类型
	unsigned short version;		// 版本号
	unsigned short not_use;
}__attribute__((packed));


void APP_MOTOR_send_get_status(unsigned char id);
void APP_MOTOR_poweron(unsigned char id);
void APP_MOTOR_poweroff(unsigned char id);
void APP_MOTOR_stop(unsigned char id);
void APP_MOTOR_set_code(unsigned char id, int code);
void APP_MOTOR_set_zero_offset(unsigned char id, int offset);
void APP_MOTOR_set_position(unsigned char id, int position);
void APP_MOTOR_set_direction(unsigned char id, unsigned char move_dir, unsigned char find_zero_dir);
void APP_MOTOR_set_code_ratio(unsigned char id, unsigned short denominator, unsigned short molecule);
void APP_MOTOR_set_zero_type(unsigned char id, unsigned char type);
void APP_MOTOR_set_zero_report(unsigned char id, unsigned char enable);
void APP_MOTOR_find_zero(unsigned char id, unsigned short speed, unsigned short torque);
void APP_MOTOR_bump_zero(unsigned char id, unsigned short speed, unsigned short torque);
void APP_MOTOR_set_bump_zero_report(unsigned char id);
void APP_MOTOR_move_position(unsigned char id, unsigned short speed, int position, unsigned char ack_flag);
void APP_MOTOR_move_speed(unsigned char id, short speed, unsigned short torque);

void APP_MOTOR_init(void);
void APP_MOTOR_loop_1ms(void);

#endif	// _APP_MOTOR_H_



