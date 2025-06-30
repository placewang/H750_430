#ifndef _APP_CMD_H_
#define _APP_CMD_H_

#define UART_STATUS_USERNAME	0
#define UART_STATUS_PASSWORD	1
#define UART_STATUS_ROOT		2
#define UART_STATUS_CONFIG		3
#define UART_STATUS_MOTOR		4

#define INPUT_STATUS_IDLE		0
#define INPUT_STATUS_ESC		1
#define INPUT_STATUS_4F5B		2
#define INPUT_STATUS_7E_END1	3
#define INPUT_STATUS_7E_END2	4

#define UART_TX_DATA_LEN		256
#define UART_TX_DATA_LOOP		0x00FF

#define UART_AGEING_TIME		300

struct UART_CONFIG
{
	char username[32];
	char password[32];
};

struct UART_DEV
{
	struct UART_CONFIG uart_config;
	
	char login_user[32];
	char prompt[64];
	char line_cmd[64];
	char last_cmd[64];
	unsigned char last_len;
	unsigned char rx_data_close;
	unsigned char status;
	unsigned char input_status;
	unsigned char cmd_len;
	unsigned short ageing_count;
	unsigned char tx_data[UART_TX_DATA_LEN];
	unsigned short tx_data_wr;
	unsigned short tx_data_rd;
	unsigned char rx_data[256];
	unsigned char rx_data_wr;
	unsigned char rx_data_rd;
	
	unsigned char motor_id;
};

struct UART_CMD_PRASE
{
	unsigned char num;
	char *cmd_p[8];
};

void APP_CMD_init(void);
void APP_CMD_loop(void);
void APP_CMD_loop_1ms(void);

#endif	// _APP_CMD_H_

