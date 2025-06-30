#include <string.h>
#include <stdio.h>
#include "main.h"
#include "bsp_uart.h"
#include "bsp_mx25l_spi.h"
#include "bsp_encoder.h"
#include "app_cmd.h"
#include "app_version.h"
#include "app_config.h"


const char default_username[] = "admin";
const char default_password[] = "123456";
const char str_welcome1[] = "===================================================\r\n";
const char str_welcome2[] = "  Welcome to the configuration terminal interface  \r\n";
const char str_welcome3[] = "===================================================\r\n";
const char str_separate[] = "---------------------------------------------------\r\n";

struct UART_DEV *G_uart_dev_p;
struct UART_DEV G_uart_dev;

unsigned char G_motor_move_index = 0;


void APP_print_version(void)
{
	unsigned char version_h;
	unsigned char version_l;
	
	version_h = APP_VERSION_H;
	version_l = APP_VERSION_L;
	printf("  Version: %d,%d\r\n", version_h, version_l);
	printf("---------------------------------------------------\r\n");
}


void APP_CMD_prase(char *line_p, struct UART_CMD_PRASE *cmd_prase_p)
{
	unsigned char i;
	
	if (cmd_prase_p == NULL)
	{
		return;
	}
	
	cmd_prase_p->num = 0;
	if ((line_p == NULL) || (*line_p == 0))
	{
		return;
	}
	
	// delete first space
	for (i = 0; i < 64; i++)
	{
		if (*line_p == 0)
		{
			return;
		}
		else if (*line_p != ' ')
		{
			break;
		}
		line_p++;
	}
	
	while (cmd_prase_p->num < 8)
	{// max 8 cmd
		cmd_prase_p->cmd_p[cmd_prase_p->num] = line_p;
		cmd_prase_p->num++;
		for (i = 0; i < 64; i++)
		{
			if (*line_p == 0)
			{
				return;
			}
			else if (*line_p == ' ')
			{
				*line_p = 0;
				line_p++;
				break;
			}
			line_p++;
		}
		
		for (i = 0; i < 64; i++)
		{
			if (*line_p == 0)
			{
				return;
			}
			else if (*line_p != ' ')
			{
				break;
			}
			line_p++;
		}
	}
}

void APP_CMD_update_prompt(unsigned char status)
{
	switch(G_uart_dev_p->status)
	{
		case UART_STATUS_USERNAME:
			strcpy(G_uart_dev_p->prompt, "username:");
			break;
		
		case UART_STATUS_PASSWORD:
			strcpy(G_uart_dev_p->prompt, "password:");
			break;
		
		case UART_STATUS_ROOT:
			sprintf(G_uart_dev_p->prompt, "[%s@root]#", G_uart_dev_p->login_user);
			break;
		
		case UART_STATUS_CONFIG:
			sprintf(G_uart_dev_p->prompt, "[%s@configure]#", G_uart_dev_p->login_user);
			break;
		
		case UART_STATUS_MOTOR:
			sprintf(G_uart_dev_p->prompt, "[%s@motor-%d]#", G_uart_dev_p->login_user, G_uart_dev_p->motor_id);
			break;
		
		default:
			sprintf(G_uart_dev_p->prompt, "%s:", G_uart_dev_p->login_user);
			break;
	}
}

void APP_CMD_deal_debug(struct UART_CMD_PRASE *prase_p)
{

}

void APP_CMD_init(void)
{
	G_uart_dev_p = &G_uart_dev;
	
//	G_uart_dev_p->rx_data_rd = 0;
//	G_uart_dev_p->rx_data_wr = 0;
	G_uart_dev_p->tx_data_rd = 0;
	G_uart_dev_p->tx_data_wr = 0;
	G_uart_dev_p->rx_data_close = 0;
	G_uart_dev_p->status = UART_STATUS_USERNAME;
	G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
	G_uart_dev_p->cmd_len = 0;
	
	strcpy(G_uart_dev_p->uart_config.username, default_username);
	strcpy(G_uart_dev_p->uart_config.password, default_password);
	
	APP_CMD_update_prompt(G_uart_dev_p->status);
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("%s", (char *)str_welcome1);
	printf("%s", (char *)str_welcome2);
	APP_print_version();
	printf("%s", G_uart_dev_p->prompt);;
}

void APP_CMD_deal_run(struct UART_CMD_PRASE *prase_p)
{
	if ((prase_p == NULL) || (prase_p->num <= 1))
	{
		return;
	}
	
	
}


void APP_CMD_deal_set(struct UART_CMD_PRASE *prase_p)
{
//	int rate;
//	int result;
	if ((prase_p == NULL) || (prase_p->num <= 1))
	{
		return;
	}

	{
		printf("ERROR:Format invalid!\r\n");
	}
}

void APP_CMD_deal_show(struct UART_CMD_PRASE *prase_p)
{
	unsigned short data;
	if ((prase_p == NULL) || (prase_p->num <= 1))
	{
		return;
	}
	
	if (prase_p->num == 2)
	{
		if (strcmp(prase_p->cmd_p[1], "version") == 0)
		{
			APP_print_version();
		}
		else if (strcmp(prase_p->cmd_p[1], "encoder") == 0)
		{
			data = BSP_ENCODER_getCounter();
			printf(" Encoder: %u\r\n", data);
		}
		else
		{
			printf("ERROR:Format invalid!\r\n");
		}
	}
	else
	{
		printf("ERROR:Format invalid!\r\n");
	}
}

void APP_CMD_deal_help(void)
{
	switch(G_uart_dev_p->status)
	{
		case UART_STATUS_USERNAME:
			break;
		
		case UART_STATUS_PASSWORD:
			break;
		
		case UART_STATUS_ROOT:
			printf(" configure              - Enter configuration mode\r\n");
			printf(" show version           - Display version\r\n");
			printf(" quit                   - Return to the upper directory\r\n");
			printf(" exit                   - User exit\r\n");
			break;
		
		case UART_STATUS_CONFIG:
			printf(" show version           - Display version\r\n");
			printf(" brt clear              - Clear brt position\r\n");
			printf(" quit                   - Return to the upper directory\r\n");
			printf(" exit                   - User exit\r\n");
			break;
		
		case UART_STATUS_MOTOR:
			printf(" move f                 - Motor speed mode to move forward\r\n");
			printf(" move r                 - Motor speed mode to move reverse\r\n");
			printf(" quit                   - Return to the upper directory\r\n");
			printf(" exit                   - User exit\r\n");
			break;
		
		default:
			break;
	}
}

void APP_CMD_deal_spi(struct UART_CMD_PRASE *prase_p)
{
	unsigned int address;
	int result;
	
	if ((prase_p == NULL) || (prase_p->num <= 1))
	{
		return;
	}
	
	if (prase_p->num == 2)
	{
		if (strcmp(prase_p->cmd_p[1], "load") == 0)
		{
			APP_CONFIG_load();
		}
		else if (strcmp(prase_p->cmd_p[1], "save") == 0)
		{
			APP_CONFIG_save();
		}
		else
		{
			printf("ERROR:Format invalid!\r\n");
		}
	}
	else if (prase_p->num == 3)
	{
		if (strcmp(prase_p->cmd_p[1], "read") == 0)
		{
			result = sscanf(prase_p->cmd_p[2], "%d", &address);
			if (result == 1)
			{
				//printf("address = %08X\r\n", address);
				MX25L_SPI_print_data(address, 1);
			}
		}
		else
		{
			printf("ERROR:Format invalid!\r\n");
		}
	}
	else
	{
		printf("ERROR:Format invalid!\r\n");
	}
}

static void APP_CMD_command_deal(char *cmd_p)
{
	struct UART_CMD_PRASE cmd_prase;
	
	if ((cmd_p == NULL) || (*cmd_p == 0))
	{
		return;
	}
	
	APP_CMD_prase(cmd_p, &cmd_prase);
	if (cmd_prase.num == 0)
	{// no cmd
		return;
	}
	
	switch(G_uart_dev_p->status)
	{
		case UART_STATUS_USERNAME:
			if (cmd_prase.num == 1)
			{
				strcpy(G_uart_dev_p->login_user, cmd_prase.cmd_p[0]);
				G_uart_dev_p->status = UART_STATUS_PASSWORD;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else
			{
				printf("username invalid!\r\n");
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			break;
		
		case UART_STATUS_PASSWORD:
			if (cmd_prase.num == 1)
			{
				if ((strcmp(G_uart_dev_p->login_user, "root") == 0) && (strcmp(cmd_prase.cmd_p[0], "159357") == 0))
				{
					G_uart_dev_p->status = UART_STATUS_ROOT;
					APP_CMD_update_prompt(G_uart_dev_p->status);
					printf(" User [root] login succeeded!\r\n");
				}
				else if ((strcmp(G_uart_dev_p->login_user, G_uart_dev_p->uart_config.username) == 0) && (strcmp(cmd_prase.cmd_p[0], G_uart_dev_p->uart_config.password) == 0))
				{
					G_uart_dev_p->status = UART_STATUS_ROOT;
					APP_CMD_update_prompt(G_uart_dev_p->status);
					printf(" User [admin] login succeeded!\r\n");
				}
				else
				{
					G_uart_dev_p->status = UART_STATUS_USERNAME;
					printf("username or password incorrect!\r\n");
					APP_CMD_update_prompt(G_uart_dev_p->status);
				}
			}
			else
			{
				G_uart_dev_p->status = UART_STATUS_USERNAME;
				printf("username or password incorrect!\r\n");
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			break;
		
		case UART_STATUS_ROOT:
			if (strcmp(cmd_prase.cmd_p[0], "configure") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_CONFIG;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "quit") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_USERNAME;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "exit") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_USERNAME;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "reboot") == 0)
			{
				//__set_FAULTMASK(1); // 关闭所有中端
				NVIC_SystemReset(); // 复位
			}
			else if (strcmp(cmd_prase.cmd_p[0], "help") == 0)
			{
				APP_CMD_deal_help();
			}
			else if (strcmp(cmd_prase.cmd_p[0], "show") == 0)
			{
				APP_CMD_deal_show(&cmd_prase);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "debug") == 0)
			{
				APP_CMD_deal_debug(&cmd_prase);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "run") == 0)
			{
				APP_CMD_deal_run(&cmd_prase);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "spi") == 0)
			{
				APP_CMD_deal_spi(&cmd_prase);
			}
			else
			{
				printf("No such command!\r\n");
			}
			break;
		
		case UART_STATUS_CONFIG:
			if (strcmp(cmd_p, "quit") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_ROOT;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "exit") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_USERNAME;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "reboot") == 0)
			{
				NVIC_SystemReset(); // 复位
			}
			else if (strcmp(cmd_prase.cmd_p[0], "help") == 0)
			{
				APP_CMD_deal_help();
			}
			else if (strcmp(cmd_prase.cmd_p[0], "show") == 0)
			{
				APP_CMD_deal_show(&cmd_prase);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "set") == 0)
			{
				APP_CMD_deal_set(&cmd_prase);
			}
			else
			{
				printf("No such command!\r\n");
			}
			break;
			
		case UART_STATUS_MOTOR:
			if (strcmp(cmd_p, "quit") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_ROOT;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "exit") == 0)
			{
				G_uart_dev_p->status = UART_STATUS_USERNAME;
				APP_CMD_update_prompt(G_uart_dev_p->status);
			}
			else if (strcmp(cmd_prase.cmd_p[0], "reboot") == 0)
			{
				
			}
			else if (strcmp(cmd_prase.cmd_p[0], "help") == 0)
			{
				APP_CMD_deal_help();
			}
			else if (strcmp(cmd_prase.cmd_p[0], "show") == 0)
			{
				APP_CMD_deal_show(&cmd_prase);
			}
			else
			{
				printf("No such command!\r\n");
			}
			break;
		
		default:
			G_uart_dev_p->status = UART_STATUS_USERNAME;
			break;
	}
}

// ESC:			1B
// F1:			1B 4F 50
// F2:			1B 4F 51	
// F3:			1B 4F 52
// F4:			1B 4F 53
// UP:			1B 5B 41
// DOWN:		1B 5B 42
// LEFT:		1B 5B 44
// RIGHT:		1B 5B 43
// INSERT:		1B 5B 32 7E
// DELETE:		1B 5B 33 7E
// HOME:		1B 5B 31 7E
// END:			1B 5B 34 7E
// PAGE UP:		1B 5B 35 7E
// PAGE DOWN:	1B 5B 36 7E
// F5:			1B 5B 31 35 7E
// F6:			1B 5B 31 37 7E
// F7:			1B 5B 31 38 7E
// F8:			1B 5B 31 39 7E
// F9:			1B 5B 32 30 7E
// F10:			1B 5B 32 31 7E
// F11:			1B 5B 32 33 7E
// F12:			1B 5B 32 34 7E
// ctrl-c:		03

void APP_CMD_uart_data_deal(char data)
{
	G_uart_dev_p->ageing_count = UART_AGEING_TIME;
	
	if (data == 0x0D)
	{// do line cmd
		printf("\r\n");
		G_uart_dev_p->line_cmd[G_uart_dev_p->cmd_len] = 0;
		strcpy(G_uart_dev_p->last_cmd, G_uart_dev_p->line_cmd);
		G_uart_dev_p->last_len = G_uart_dev_p->cmd_len;
		APP_CMD_command_deal(G_uart_dev_p->line_cmd);
		printf("%s", G_uart_dev_p->prompt);;
		G_uart_dev_p->cmd_len = 0;
		G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
	}
	else if (data == 0x03)
	{// cancel
		printf("\r\n");
		printf("%s", G_uart_dev_p->prompt);;
		G_uart_dev_p->cmd_len = 0;
		G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
	}
	else if (data == '?')
	{
		if ((G_uart_dev_p->status != UART_STATUS_PASSWORD) &&
			(G_uart_dev_p->status != UART_STATUS_USERNAME))
		{
			printf("\r\n");
			APP_CMD_deal_help();
			printf("%s", G_uart_dev_p->prompt);;
			G_uart_dev_p->line_cmd[G_uart_dev_p->cmd_len] = 0;	// END 
			printf("%s", G_uart_dev_p->line_cmd);
		}
	}
	else
	{
		if (G_uart_dev_p->input_status == INPUT_STATUS_IDLE)
		{
			if (data == 0x1B)
			{
				G_uart_dev_p->input_status = INPUT_STATUS_ESC;
			}
			else
			{
//				if (((data >= '0') && (data <= '9')) ||
//					((data >= 'A') && (data <= 'Z')) ||
//					((data >= 'a') && (data <= 'z')))
				if ((data >= ' ') && (data <= '~'))
				{
					if (G_uart_dev_p->cmd_len < 64)
					{
						G_uart_dev_p->line_cmd[G_uart_dev_p->cmd_len] = data;
						G_uart_dev_p->cmd_len++;
						
						if (G_uart_dev_p->status == UART_STATUS_PASSWORD)
							G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = '*';
						else
							G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = data;
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
					}
				}
				else if (data == 0x08)
				{
					if (G_uart_dev_p->cmd_len != 0)
					{
						G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = 0x08;
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
						G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = ' ';
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
						G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = 0x08;
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
						G_uart_dev_p->cmd_len--;
					}
				}
			}
		}
		else if (G_uart_dev_p->input_status == INPUT_STATUS_ESC)
		{
			if ((data == 0x4F) || (data == 0x5B))
			{
				G_uart_dev_p->input_status = INPUT_STATUS_4F5B;
			}
			else if (data == 0x1B)
			{
				G_uart_dev_p->input_status = INPUT_STATUS_ESC;
			}
			else
			{
//				if (((data >= '0') && (data <= '9')) ||
//					((data >= 'A') && (data <= 'Z')) ||
//					((data >= 'a') && (data <= 'z')))
				if ((data >= ' ') && (data <= '~'))
				{
					if (G_uart_dev_p->cmd_len < 64)
					{
						G_uart_dev_p->line_cmd[G_uart_dev_p->cmd_len] = data;
						G_uart_dev_p->cmd_len++;
						
						if (G_uart_dev_p->status == UART_STATUS_PASSWORD)
							G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = '*';
						else
							G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = data;
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
					}
				}
				else if (data == 0x08)
				{
					if (G_uart_dev_p->cmd_len != 0)
					{
						G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = 0x08;
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
						G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = ' ';
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
						G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_wr] = 0x08;
						G_uart_dev_p->tx_data_wr++;
						G_uart_dev_p->tx_data_wr = G_uart_dev_p->tx_data_wr & UART_TX_DATA_LOOP;
						G_uart_dev_p->cmd_len--;
					}
				}
				G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
			}
		}
		else if (G_uart_dev_p->input_status == INPUT_STATUS_4F5B)
		{
			if (data >= 0x40)
			{
				if (data == 0x41)
				{// UP 
					if (G_uart_dev_p->last_len > 0)
					{
						printf("\r\n");
						printf("%s", G_uart_dev_p->prompt);
						strcpy(G_uart_dev_p->line_cmd, G_uart_dev_p->last_cmd);
						G_uart_dev_p->cmd_len = G_uart_dev_p->last_len;
						printf("%s", G_uart_dev_p->line_cmd);
					}
				}
				G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
			}
			else
			{
				G_uart_dev_p->input_status = INPUT_STATUS_7E_END1;
			}
		}
		else if (G_uart_dev_p->input_status == INPUT_STATUS_7E_END1)
		{
			if (data == 0x7E)
			{
				G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
			}
			else
			{
				G_uart_dev_p->input_status = INPUT_STATUS_7E_END2;
			}
		}
		else
		{
			G_uart_dev_p->input_status = INPUT_STATUS_IDLE;
		}
	}
}

//void APP_CMD_recv_data(unsigned char data)
//{
//	G_uart_dev_p->rx_data[G_uart_dev_p->rx_data_wr] = data;
//	G_uart_dev_p->rx_data_wr++;
//}


void APP_CMD_loop_1s(void)
{
	if (G_uart_dev_p->ageing_count)
	{
		G_uart_dev_p->ageing_count--;
		if (G_uart_dev_p->ageing_count == 0)
		{
			G_uart_dev_p->status = UART_STATUS_USERNAME;
			printf("\r\nLogin timeout, auto exit!\r\n");
			APP_CMD_update_prompt(G_uart_dev_p->status);
			printf("%s", G_uart_dev_p->prompt);;
		}
	}
}

void APP_CMD_loop_1ms(void)
{
	static unsigned short count = 0;
	unsigned char result;
	unsigned char data;
	
	if (G_uart_dev_p->tx_data_wr != G_uart_dev_p->tx_data_rd)
	{// 发送串口数据
		BSP_UART4_send_byte(G_uart_dev_p->tx_data[G_uart_dev_p->tx_data_rd]);
		G_uart_dev_p->tx_data_rd++;
		G_uart_dev_p->tx_data_rd = G_uart_dev_p->tx_data_rd & UART_TX_DATA_LOOP;
	}
	
	result = BSP_UART4_read_byte(&data);
	if (result == 0)
	{
		APP_CMD_uart_data_deal(data);
	}
	
	count++;
	if (count >= 1000)
	{
		count = 0;
		APP_CMD_loop_1s();
	}
}



/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
