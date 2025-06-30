#ifndef _APP_HOT_PRESS_H_
#define _APP_HOT_PRESS_H_

#define HOT_PRESS_MOTOR_VERTICAL_MAX_NUM		4		// �����������

#define HOT_PRESS_EVENT_MIN_MOVE_POSITION		20		// ����ƶ�����С���룬С�ڸþ���ʱ�����ƶ������Ҳ��Ϊ����˶��ж����ٽ�ֵ
#define HOT_PRESS_EVENT_MAX_NUM					256		// ����¼�����

#define HOT_PRESS_MOTOR_TYPE_NULL				0x00	// ��
#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
#define HOT_PRESS_MOTOR_TYPE_MAX				0x03	// ���


// ���ж���֮ǰ������Ҫ���»�ȡλ��
#define HOT_PRESS_MOTOR_STATUS_IDLE				0x00	// �ϵ�״̬����
#define HOT_PRESS_MOTOR_STATUS_INIT_S1			0x01	// ��ʼ�����裬�жϵ���Ƿ���ڣ�����71����
#define HOT_PRESS_MOTOR_STATUS_INIT_S2			0x02	// ��ʼ�����裬�µ�
#define HOT_PRESS_MOTOR_STATUS_INIT_S3			0x03	// ��ʼ�����裬���ù��㷽ʽ
#define HOT_PRESS_MOTOR_STATUS_INIT_S4			0x04	// ��ʼ�����裬���÷���
#define HOT_PRESS_MOTOR_STATUS_INIT_S5			0x05	// ��ʼ�����裬���ñ���������
#define HOT_PRESS_MOTOR_STATUS_INIT_S5_1		0x06	// ��ʼ�����裬���ñ���������
#define HOT_PRESS_MOTOR_STATUS_INIT_S5_2		0x07	// ��ʼ�����裬������λ״̬�仯�ϱ�
#define HOT_PRESS_MOTOR_STATUS_INIT_S6			0x08	// ��ʼ�����裬�ϵ�
#define HOT_PRESS_MOTOR_STATUS_INIT_S7			0x09	// ��ʼ�����裬�ϵ�

#define HOT_PRESS_MOTOR_STATUS_RUNNING			0x10	// ������

#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S1		0x20	// ���㲽��1�����»�ȡλ��
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S2		0x21	// ���㲽��2������λ������
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S3		0x22	// ���㲽��3���ҵ������ƶ�һ��
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S4		0x23	// ���㲽��4��ֹͣһС��ʱ��
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S5		0x24	// ���㲽��5�������ƶ����Ƴ���λ��������ֹͣ���ӳ�100ms
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S6		0x25	// ���㲽��6������λ�ã�
#define HOT_PRESS_MOTOR_STATUS_FIND_ZERO_S7		0x26	// ���㲽��7���ƶ���1000λ��

#define HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S1	0x30	// �ٶ�ģʽ���У����»�ȡλ��
#define HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S2	0x31	// �ٶ�ģʽ���У�����
#define HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S3	0x32	// �ٶ�ģʽ���У���ס

#define HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S1	0x33	// λ��ģʽ���У���ȡ��ǰλ��
#define HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S2	0x34	// λ��ģʽ���У�����
#define HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S3	0x35	// λ��ģʽ���У���ס

#define HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S1	0x36	// λ��ģʽ���У���ȡ��ǰλ��
#define HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S2	0x37	// λ��ģʽ���У�����
#define HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S3	0x38	// λ��ģʽ���У�����

#define HOT_PRESS_MOTOR_STATUS_MOVE_TRIM_S1		0x39	// λ��ģʽ��΢��
#define HOT_PRESS_MOTOR_STATUS_MOVE_FOLLOW_S1	0x3A	// λ��ģʽ������

#define HOT_PRESS_MOTOR_STATUS_FAILED_0			0xE0	// ���������
#define HOT_PRESS_MOTOR_STATUS_FAILED_1			0xE1	// ��ʼ��ʧ��
#define HOT_PRESS_MOTOR_STATUS_FAILED_2			0xE2	// ����ʧ��
#define HOT_PRESS_MOTOR_STATUS_FAILED_3			0xE3	// ����ƶ��쳣
#define HOT_PRESS_MOTOR_STATUS_FAILED_4			0xE4	// δ��⵽�������ź�

// �ظ���־
#define HOT_PRESS_MOTOR_EVENT_NULL				0x00	// ��
#define HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS		0x01	// �յ���Ӧ�Ļظ�
#define HOT_PRESS_MOTOR_EVENT_TIMEOUT			0x02	// δ�յ���Ӧ�Ļظ�����ʱ
#define HOT_PRESS_MOTOR_EVENT_GET_SIG1			0x03	// ��ȡ���ź�1
#define HOT_PRESS_MOTOR_EVENT_GET_SIG2			0x04	// ��ȡ���ź�2
#define HOT_PRESS_MOTOR_EVENT_MOTOR_MOVE_ERROR	0x05	// ����쳣���ƶ�����

#define HOT_PRESS_MOTOR_EVENT_START_FIND_ZERO	0x10	// ��������
#define HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE	0x11	// �����ٶ�ģʽ
#define HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE	0x12	// �ر��ٶ�ģʽ
#define HOT_PRESS_MOTOR_EVENT_START_OFFSET_MOVE	0x13	// ����λ��ģʽ��ƫ��
#define HOT_PRESS_MOTOR_EVENT_START_POSIT_MOVE	0x14	// ����λ��ģʽ��λ��
#define HOT_PRESS_MOTOR_EVENT_START_TRIM_MOVE	0x15	// ����λ��ģʽ��΢��
#define HOT_PRESS_MOTOR_EVENT_MOTOR_STOP		0x16	// ���ֹͣ
#define HOT_PRESS_MOTOR_EVENT_START_FOLLOW_MOVE	0x17	// ����λ��ģʽ�������ƶ�
//#define HOT_PRESS_MOTOR_EVENT_LAST_FOLLOW_MOVE	0x17	// ����λ��ģʽ�������ƶ������һ��


// �ٶ�ģʽʱ��Ĭ���ƶ�����
#define HOT_PRESS_MOTOR_SPEED_ZEOR_DEFAULT_SPEED		200			// �ٶ�ģʽ������ʱ��Ĭ���ٶ�
#define HOT_PRESS_MOTOR_SPEED_ZEOR_DEFAULT_TORQUE		950			// �ٶ�ģʽ������ʱ��Ĭ������
#define HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_SPEED		200			// �ٶ�ģʽ���ƶ�ʱ��Ĭ���ٶ�
#define HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_TORQUE		950			// �ٶ�ģʽ���ƶ�ʱ��Ĭ������
#define HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_SPEED		10			// �ٶ�ģʽ��΢��ʱ��Ĭ���ٶ�
#define HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_TORQUE		950			// �ٶ�ģʽ��΢��ʱ��Ĭ������
#define HOT_PRESS_MOTOR_POSITION_MOVE_DEFAULT_SPEED		200			// λ��ģʽ���ƶ�ʱ��Ĭ���ٶ�

#define HOT_PRESS_MOTOR_EXIST_LIVE_TIME					18000		// ������ߵ�����ʱ��
//#define HOT_PRESS_MOTOR_GET_POSITION_TIME				2000		// ��ʱ��ȡλ�õ�ʱ��
#define HOT_PRESS_MOTOR_GET_POSITION_TIME				500			// ��ʱ��ȡλ�õ�ʱ��
#define HOT_PRESS_MOTOR_FAST_UPDATE_POSITION_TIME		50			// ����ˢ��λ�ã�200msһ��

// ����˶�������
#define MOTOR_MOVE_CHECK_COUNT_UPDATE_POSITION			100			// �ڶ��η��͸���λ��
#define MOTOR_MOVE_CHECK_COUNT_GET_POSITION				105			// �ڶ��λ�ȡλ��


struct HOT_PRESS_MOTOR
{
	unsigned char enable;			// ʹ��
	unsigned char id;				// ����Ŀ���ID
	unsigned char type;				// ���������
	unsigned char index;			// ������͵���ţ�ÿ�����ʹ�0��ʼ
	unsigned char status;			// ��ǰ״̬
	unsigned char move_direction;	// ���ò���������˶�����
	unsigned short denominator;		// ������������ĸ
	unsigned short molecule;		// ��������������
	
	unsigned char reply_flag;		// �ظ���־
	unsigned char expected_reply;	// �յ��Ļظ�����
	unsigned char f_limit_enable;	// ������λ���أ�ʹ�ܣ�ʹ�ô�����1
	unsigned char r_limit_enable;	// ������λ���أ�ʹ�ܣ�ʹ�ô�����2
	
	unsigned char motor_signal_status;	// ��ǰ�������ź�
	unsigned char motor_signal2_status;	// ��ǰ������2�ź�
	
	unsigned char retry_count;			// ���Լ�����
	unsigned short time_count;			// ��ʱ����1msÿ��
	unsigned short exist_count;			// ��λ��ʱ�����յ���Ϣ���ã���ʱ����Ϊ�������
	int set_position;					// �趨λ��
	int crt_position;					// ��ǰλ��
	int target_position;				// Ŀ��λ��
	
	short move_speed;					// �ƶ��ٶ�
	short speed_mode_move_speed;		// �ٶ�ģʽ���ƶ�ʱ���ٶ�
	short speed_mode_move_torque;		// �ٶ�ģʽ���ƶ�ʱ������
	short speed_mode_trim_speed;		// �ٶ�ģʽ��΢��ʱ���ٶ�
	short speed_mode_trim_torque;		// �ٶ�ģʽ��΢��ʱ������
	unsigned short position_mode_speed;	// λ��ģʽ���ƶ�ʱ���ٶ�
	
	// ����ˢ��λ��
	unsigned char fast_update_position_enable;	// ʹ��
	unsigned char fast_update_position_delay_disable;	// �ӳٹر�
	unsigned char fast_update_position_time;	// ����ˢ�¶�ʱ��
	unsigned char running_dir;
	
	// ��������
	short running_speed;				// ��ǰ�����ٶ�	
	int running_position_offset;		// ��ǰ����λ�ò�ֵ
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
// ����
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

// ����
void APP_HP_init(void);
void APP_HP_msg_deal(unsigned short id, unsigned char *data_p, unsigned short len);
void APP_HP_loop_1ms(void);


struct HOT_PRESS_MOTOR *APP_HP_get_motor_point_by_id(unsigned char id);

unsigned char APP_HP_get_motor_status(unsigned char type, unsigned char index);
#endif	// _APP_HOT_PRESS_H_

