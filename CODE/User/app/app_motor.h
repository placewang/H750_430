#ifndef _APP_MOTOR_H_
#define _APP_MOTOR_H_

#define MOTOR_SEND_CAN_ID					0x0001		// �ջ�������͵�CAN ID


// ��ǿ�ջ�����������ֽڶ���
#define CMD_HQ_MOTOR_FIND_ZERO_BY_HIT		0x01		// ײ��(���ײͣʱ������λ)
#define CMD_HQ_MOTOR_SET_PARAMETER			0x02		// ���û�������

#define CMD_HQ_MOTOR_YARN_FORWARD_FOLLOW	0x03		// ɴ���������
#define CMD_HQ_MOTOR_YARN_REVERSE_FOLLOW	0x04		// ɴ�췴�����
#define CMD_HQ_MOTOR_YARN_STOP_FOLLOW		0x05		// ɴ��ֹͣ����
#define CMD_HQ_MOTOR_YARN_MOVE				0x06		// ɴ���ƶ���

#define CMD_HQ_MOTOR_ENABLE					0x07		// ���ʹ��
#define CMD_HQ_MOTOR_MOVE_P_WITHOUT_ACK		0x0A		// λ���ƶ�
#define CMD_HQ_MOTOR_MOVE_S_WITHOUT_ACK		0x0B		// �ٶ��ƶ�
#define CMD_HQ_MOTOR_MOVE_STOP_NOW			0x11		// ����˶�ֹͣ��������λ��
#define CMD_HQ_MOTOR_MOVE_P_WITH_ACK		0x8A		// ����λ�ظ���λ���ƶ�
#define CMD_HQ_MOTOR_GET_STATUS				0x71		// ��ȡ���״̬
#define CMD_HQ_MOTOR_READ_PARAMETER			0x72		// ��ȡ�������
#define CMD_HQ_MOTOR_SET_ARRIVAL_IND		0x78		// ���õ�λ֪ͨ��Ŀǰ�õ����㵽λ֪ͨ������������ٷ���
#define CMD_HQ_MOTOR_RESET_ARRIVAL_IND		0x78		// ��λ�ĵ�λ֪ͨ����Ҫ������
#define CMD_HQ_MOTOR_MOVE_ARRIVAL_IND		0x15		// �ƶ��ĵ�λ֪ͨ
#define CMD_HQ_MOTOR_FIND_ZERO_BY_SENSOR	0x31		// ����(��������ʽ)

#define CMD_HQ_MOTOR_YARN_SET_ID_MODE		0x50		// ������˳�����IDģʽ
#define CMD_HQ_MOTOR_YARN_GET_DID			0x51		// ��ȡ�豸�����ʶ��
#define CMD_HQ_MOTOR_YARN_SET_LED_MODE		0x52		// ѡ�в������ƣ�500ms��
#define CMD_HQ_MOTOR_YARN_SET_MOTOR_ID		0x53		// ���õ����Motor_ID

#define CMD_HQ_MOTOR_YARN_GET_VERSION		0xE7		// ��ȡ����İ汾��


// ������ò�������
#define MOTOR_PARAMETER_TYPE_SET_CODE			0x8001		// ���ñ�����ֵ 
#define MOTOR_PARAMETER_TYPE_SET_DIR			0x8002		// ���÷���
#define MOTOR_PARAMETER_TYPE_SET_OFFSET			0x8003		// ����ײ����ƶ���ƫ����
#define MOTOR_PARAMETER_TYPE_SET_CODE_RATIO		0x8005		// ���ñ������������
#define MOTOR_PARAMETER_TYPE_SET_OFFSET_RATIO	0x8007		// ����ƫ��������
#define MOTOR_PARAMETER_TYPE_SET_POSITION		0x8008		// ���õ��λ��
#define MOTOR_PARAMETER_TYPE_FIND_ZERO_TYPE		0x8009		// �������㷽ʽ
#define MOTOR_PARAMETER_TYPE_MAX_SPEED			0x800B		// ���õ���������
#define MOTOR_PARAMETER_TYPE_SET_ZERO_REPORT	0x8025		// ������λ������״̬�Զ��ϱ�


// ���ò�������Ϣ�ṹ��
struct LOOP_MOTOR_ACK_GET_STATUS
{
	unsigned char cmd;						// ����
	unsigned char id;						// ID
	unsigned char error;					// ����̬
	unsigned char signal;					// �ź�״̬(����0��1λ��ʾ�����źţ�1��ʾͨ 0��ʾ��ͨ����2λ��ʾ�հ��źţ�1��ʾ�հ� 0��ʾ�����ң�)
	int position;							// ��ǰλ��
}__attribute__((packed));


// �������������
union LOOP_MOTOR_PARAMETER
{
	int data_32;
	unsigned int udata_32;
	short data_16[2];
	unsigned short udata_16[2];
	char data_8[4];
	unsigned char udata_8[4];
}__attribute__((packed));

// ���ò�������Ϣ�ṹ��
struct LOOP_MOTOR_MSG_SET_PARAMETER
{
	unsigned char cmd;						// ����
	unsigned char id;						// ID
	unsigned short parameter_type;			// ��������
	union LOOP_MOTOR_PARAMETER parameter;	// ���õĲ���
}__attribute__((packed));

// �������
struct LOOP_MOTOR_MSG_FIND_ZERO
{
	unsigned char cmd;			// ����
	unsigned char id;			// ID
	short speed;				// �ٶȣ��������ֵ�İٷֱȣ�-100��100 ����������
	unsigned short torque;		// Ť�ص�ǧ�ֱ�
}__attribute__((packed));


// ����ƶ�����Ϣ�ṹ��λ���ƶ�
struct LOOP_MOTOR_MSG_MOVE_P
{
	unsigned char cmd;			// ����
	unsigned char id;			// ID
	unsigned short speed;		// �ٶȣ�Ȧ/��
	unsigned int position;		// �ƶ���Ŀ��λ��
}__attribute__((packed));

// ����ƶ�����Ϣ�ṹ���ٶ��ƶ�
struct LOOP_MOTOR_MSG_MOVE_S
{
	unsigned char cmd;			// ����
	unsigned char id;			// ID
	unsigned short torque;		// Ť�ص�ǧ�ֱ�
	int speed;					// �ƶ����ٶ�
}__attribute__((packed));

// ���״̬�ظ�����Ϣ�ṹ
struct LOOP_MOTOR_MSG_STATUS_ACK
{
	unsigned char cmd;			// ����
	unsigned char id;			// ID
	unsigned char error;		// ����̬
	unsigned char signal;		// �ź�״̬(����0��1λ��ʾ�����źţ�1��ʾͨ 0��ʾ��ͨ����2λ��ʾ�հ��źţ�1��ʾ�հ� 0��ʾ�����ң�)
	unsigned int position;		// ��ǰλ��
}__attribute__((packed));

// �������ID����Ϣ�ṹ
struct LOOP_MOTOR_MSG_SET_ID
{
	unsigned char cmd;			// ����
	unsigned char id;			// ID
	unsigned char mode;			// ģʽ
	unsigned char data;			// ����
	unsigned int did;			// �豸ID
}__attribute__((packed));


// �����ȡ�汾����Ϣ�ṹ
struct LOOP_MOTOR_MSG_GET_VERSION
{
	unsigned char cmd;			// ����
	unsigned char id;			// ID
	unsigned short device_type;	// �������
	unsigned short version;		// �汾��
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



