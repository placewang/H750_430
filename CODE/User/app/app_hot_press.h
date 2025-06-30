#ifndef _APP_HOT_PRESS_H_
#define _APP_HOT_PRESS_H_

#define HOT_PRESS_MOTOR_VERTICAL_MAX_NUM		4		// 最大竖烫数量

#define HOT_PRESS_EVENT_MIN_MOVE_POSITION		20		// 电机移动的最小距离，小于该距离时，不移动电机；也作为电机运动判定的临界值
#define HOT_PRESS_EVENT_MAX_NUM					256		// 电机事件数量

#define HOT_PRESS_MOTOR_TYPE_NULL				0x00	// 无
#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
#define HOT_PRESS_MOTOR_TYPE_MAX				0x03	// 最大


// 所有动作之前，都需要重新获取位置
#define HOT_PRESS_MOTOR_STATUS_IDLE				0x00	// 上电状态，空
#define HOT_PRESS_MOTOR_STATUS_INIT_S1			0x01	// 初始化步骤，判断电机是否存在，发送71命令
#define HOT_PRESS_MOTOR_STATUS_INIT_S2			0x02	// 初始化步骤，下电
#define HOT_PRESS_MOTOR_STATUS_INIT_S3			0x03	// 初始化步骤，设置归零方式
#define HOT_PRESS_MOTOR_STATUS_INIT_S4			0x04	// 初始化步骤，设置方向
#define HOT_PRESS_MOTOR_STATUS_INIT_S5			0x05	// 初始化步骤，设置编码器比例
#define HOT_PRESS_MOTOR_STATUS_INIT_S5_1		0x06	// 初始化步骤，设置编码器比例
#define HOT_PRESS_MOTOR_STATUS_INIT_S5_2		0x07	// 初始化步骤，设置零位状态变化上报
#define HOT_PRESS_MOTOR_STATUS_INIT_S6			0x08	// 初始化步骤，上电
#define HOT_PRESS_MOTOR_STATUS_INIT_S7			0x09	// 初始化步骤，上电

#define HOT_PRESS_MOTOR_STATUS_RUNNING			0x10	// 运行中

#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S1		0x20	// 找零步骤1：重新获取位置
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S2		0x21	// 找零步骤2：找零位传感器
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S3		0x22	// 找零步骤3：找到后，再移动一点
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S4		0x23	// 找零步骤4：停止一小段时间
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S5		0x24	// 找零步骤5：反向移动，移出零位传感器，停止，延迟100ms
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S6		0x25	// 找零步骤6：设置位置，
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S7		0x26	// 找零步骤7：移动到1000位置

#define HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S1	0x30	// 速度模式运行，重新获取位置
#define HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S2	0x31	// 速度模式运行，运行
#define HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S3	0x32	// 速度模式运行，锁住

#define HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S1	0x33	// 位置模式运行，获取当前位置
#define HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S2	0x34	// 位置模式运行，运行
#define HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S3	0x35	// 位置模式运行，锁住

#define HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S1	0x36	// 位置模式运行，获取当前位置
#define HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S2	0x37	// 位置模式运行，运行
#define HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S3	0x38	// 位置模式运行，运行

#define HOT_PRESS_MOTOR_STATUS_MOVE_TRIM_S1		0x39	// 位置模式，微调
#define HOT_PRESS_MOTOR_STATUS_MOVE_FOLLOW_S1	0x3A	// 位置模式，跟随

#define HOT_PRESS_MOTOR_STATUS_FAILED_0			0xE0	// 电机不在线
#define HOT_PRESS_MOTOR_STATUS_FAILED_1			0xE1	// 初始化失败
#define HOT_PRESS_MOTOR_STATUS_FAILED_2			0xE2	// 找零失败
#define HOT_PRESS_MOTOR_STATUS_FAILED_3			0xE3	// 电机移动异常
#define HOT_PRESS_MOTOR_STATUS_FAILED_4			0xE4	// 未检测到传感器信号

// 回复标志
#define HOT_PRESS_MOTOR_EVENT_NULL				0x00	// 空
#define HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS		0x01	// 收到对应的回复
#define HOT_PRESS_MOTOR_EVENT_TIMEOUT			0x02	// 未收到对应的回复，超时
#define HOT_PRESS_MOTOR_EVENT_GET_SIG1			0x03	// 获取到信号1
#define HOT_PRESS_MOTOR_EVENT_GET_SIG2			0x04	// 获取到信号2
#define HOT_PRESS_MOTOR_EVENT_MOTOR_MOVE_ERROR	0x05	// 电机异常，移动错误

#define HOT_PRESS_MOTOR_EVENT_START_FIND_ZERO	0x10	// 启动找零
#define HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE	0x11	// 启动速度模式
#define HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE	0x12	// 关闭速度模式
#define HOT_PRESS_MOTOR_EVENT_START_OFFSET_MOVE	0x13	// 启动位置模式，偏差
#define HOT_PRESS_MOTOR_EVENT_START_POSIT_MOVE	0x14	// 启动位置模式，位置
#define HOT_PRESS_MOTOR_EVENT_START_TRIM_MOVE	0x15	// 启动位置模式，微调
#define HOT_PRESS_MOTOR_EVENT_MOTOR_STOP		0x16	// 电机停止
#define HOT_PRESS_MOTOR_EVENT_START_FOLLOW_MOVE	0x17	// 启动位置模式，跟随移动
//#define HOT_PRESS_MOTOR_EVENT_LAST_FOLLOW_MOVE	0x17	// 启动位置模式，跟随移动，最后一次


// 速度模式时的默认移动参数
#define HOT_PRESS_MOTOR_SPEED_ZEOR_DEFAULT_SPEED		200			// 速度模式：找零时的默认速度
#define HOT_PRESS_MOTOR_SPEED_ZEOR_DEFAULT_TORQUE		950			// 速度模式：找零时的默认力矩
#define HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_SPEED		200			// 速度模式：移动时的默认速度
#define HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_TORQUE		950			// 速度模式：移动时的默认力矩
#define HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_SPEED		10			// 速度模式：微调时的默认速度
#define HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_TORQUE		950			// 速度模式：微调时的默认力矩
#define HOT_PRESS_MOTOR_POSITION_MOVE_DEFAULT_SPEED		200			// 位置模式：移动时的默认速度

#define HOT_PRESS_MOTOR_EXIST_LIVE_TIME					18000		// 电机在线的生存时间
//#define HOT_PRESS_MOTOR_GET_POSITION_TIME				2000		// 定时获取位置的时间
#define HOT_PRESS_MOTOR_GET_POSITION_TIME				500			// 定时获取位置的时间
#define HOT_PRESS_MOTOR_FAST_UPDATE_POSITION_TIME		50			// 快速刷新位置，200ms一次

// 电机运动检查机制
#define MOTOR_MOVE_CHECK_COUNT_UPDATE_POSITION			100			// 第二次发送更新位置
#define MOTOR_MOVE_CHECK_COUNT_GET_POSITION				105			// 第二次获取位置


struct HOT_PRESS_MOTOR
{
	unsigned char enable;			// 使能
	unsigned char id;				// 电机的控制ID
	unsigned char type;				// 电机的类型
	unsigned char index;			// 电机类型的序号，每种类型从0开始
	unsigned char status;			// 当前状态
	unsigned char move_direction;	// 设置参数：电机运动方向
	unsigned short denominator;		// 传动参数：分母
	unsigned short molecule;		// 传动参数：分子
	
	unsigned char reply_flag;		// 回复标志
	unsigned char expected_reply;	// 收到的回复命令
	unsigned char f_limit_enable;	// 正向限位开关，使能，使用传感器1
	unsigned char r_limit_enable;	// 反向限位开关，使能，使用传感器2
	
	unsigned char motor_signal_status;	// 当前传感器信号
	unsigned char motor_signal2_status;	// 当前传感器2信号
	
	unsigned char retry_count;			// 重试计数器
	unsigned short time_count;			// 定时器，1ms每次
	unsigned short exist_count;			// 在位定时器，收到消息设置；超时后，认为电机掉线
	int set_position;					// 设定位置
	int crt_position;					// 当前位置
	int target_position;				// 目标位置
	
	short move_speed;					// 移动速度
	short speed_mode_move_speed;		// 速度模式：移动时的速度
	short speed_mode_move_torque;		// 速度模式：移动时的力矩
	short speed_mode_trim_speed;		// 速度模式：微调时的速度
	short speed_mode_trim_torque;		// 速度模式：微调时的力矩
	unsigned short position_mode_speed;	// 位置模式：移动时的速度
	
	// 快速刷新位置
	unsigned char fast_update_position_enable;	// 使能
	unsigned char fast_update_position_delay_disable;	// 延迟关闭
	unsigned char fast_update_position_time;	// 快速刷新定时器
	unsigned char running_dir;
	
	// 运行数据
	short running_speed;				// 当前运行速度	
	int running_position_offset;		// 当前运行位置差值
	int running_position;				// 
};

struct HOT_PRESS_EVENT
{
	struct HOT_PRESS_MOTOR *motor_p;
	unsigned char event;
};

unsigned char APP_HP_check_all_motor_idle(void);

void APP_HP_set_motor_use_num(unsigned char motor_type, unsigned char num);

void APP_HP_set_motor_enable(unsigned char motor_type, unsigned char motor_index, unsigned char enable);
void APP_HP_set_motor_id(unsigned char motor_type, unsigned char motor_index, unsigned char id);
void APP_HP_set_motor_move_direction(unsigned char motor_type, unsigned char motor_index, unsigned char move_dir);
void APP_HP_set_motor_ratio_denominator(unsigned char motor_type, unsigned char motor_index, unsigned short denominator);
void APP_HP_set_motor_ratio_molecule(unsigned char motor_type, unsigned char motor_index, unsigned short molecule);
void APP_HP_set_motor_trim_speed(unsigned char motor_type, unsigned char motor_index, unsigned short speed);

void APP_HP_print_motor_status(void);
unsigned char APP_HP_get_motor_use_num(unsigned char type);
int APP_HP_get_motor_current_position(unsigned char motor_type, unsigned char motor_index);
int APP_HP_get_motor_current_position_by_id(unsigned char motor_id);
int APP_HP_get_motor_target_position(unsigned char motor_type, unsigned char motor_index);
int APP_HP_get_motor_target_position_by_id(unsigned char motor_id);
unsigned char APP_HP_get_zero_sensor_index(unsigned char motor_type, unsigned char motor_index);
unsigned char APP_HP_get_motor_id(unsigned char motor_type, unsigned char motor_index);
struct HOT_PRESS_MOTOR *APP_HP_get_motor_point(unsigned char motor_type, unsigned char motor_index);
// 动作
void APP_HP_motor_init(void);
void APP_HP_move_init(void);
unsigned char APP_HP_speed_move(unsigned char motor_type, unsigned char motor_index, short speed_flag);
unsigned char APP_HP_speed_move_by_id(unsigned char id, short speed_flag);
unsigned char APP_HP_trim_move_by_id(unsigned char id, short speed_flag);
unsigned char APP_HP_position_offset_move(unsigned char motor_type, unsigned char motor_index, int offset);
unsigned char APP_HP_position_offset_move_by_id(unsigned char id, int offset);
unsigned char APP_HP_position_move_by_id(unsigned char id, int position);
unsigned char APP_HP_position_trim_by_id(unsigned char id, int position);
unsigned char APP_HP_position_follow_by_id(unsigned char id, int position);
unsigned char APP_HP_motor_stop(unsigned char motor_type, unsigned char motor_index);
unsigned char APP_HP_motor_stop_by_id(unsigned char id);
void APP_HP_set_current_position(unsigned char motor_type, unsigned char motor_index, int position);

//
unsigned char APP_HP_status_check(int *motor_type_p, int *motor_index_p);

// 处理
void APP_HP_init(void);
void APP_HP_msg_deal(unsigned short id, unsigned char *data_p, unsigned short len);
void APP_HP_loop_1ms(void);


struct HOT_PRESS_MOTOR *APP_HP_get_motor_point_by_id(unsigned char id);

unsigned char APP_HP_get_motor_status(unsigned char type, unsigned char index);
#endif	// _APP_HOT_PRESS_H_

