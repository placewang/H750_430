#include <string.h>
#include <stdio.h>

#include "bsp_mx25l_spi.h"

#include "ui_my_deal.h"
#include "ui_my_config.h"

#include "app_config.h"
#include "app_DischargeSystem.h"
#include "app_balanceRoll_code.h"

/*
插入配置参数到FLASH中
pid:参数索引表
data:要插入的数据
*/
signed char  APP_AddConfigParameterToFlash(void)
{
 	unsigned int index=0;
//	unsigned int pid;
//	unsigned int data;
    
    MX25L_SPI_pid_data_init(); 
    
    MX25L_SPI_pid_data_write(index,APP_CONFIG_START_PID,APP_CONFIG_START_FLAG);
    index++;
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_RUN_MODE, Discharge.mode); 	
    index++;
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_DEV_MODE, Discharge.devMode); 	
    index++;       
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_UP_MOVE_ENAE, Discharge.UpMoveEnable); 	
    index++;    
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_UP_DRI, Discharge.UpDir); 	
    index++; 
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_UP_TENSION_POS, Discharge.UpTensionPos); 	
    index++;     
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_UP_JOG_SPEED, Discharge.UpJogSpeed); 	
    index++;       
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_UP_ENCODERFDIR, Encode_handle.EncoderUpDir); 	
    index++;      


    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_DOWN_MOVE_ENAE, Discharge.DownMoveEnable); 	
    index++;    
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_DOWN_DRI,Discharge.DownDir); 	
    index++; 
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_DOWN_TENSION_POS,Discharge.DownTensionPos); 	
    index++;     
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_DOWN_JOG_SPEED,Discharge.DownJogSpeed); 	
    index++;     
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_DOWN_ENCODERFDIR, Encode_handle.EncoderDownDir); 	
    index++;   
    MX25L_SPI_pid_data_write(index, APP_CONFIG_PID_LANGUAGE_MODE, uiCfg_Language); 	
    index++;      
    MX25L_SPI_pid_data_write(index, APP_CONFIG_END_PID, index- 1);   
    MX25L_SPI_pid_data_save();
    return 1;
}


/*****************************************************************
APP_CONFIG_load:配置参数导入
******************************************************************/
void APP_CONFIG_load(void)
{
	unsigned int index;
	unsigned int pid;
	unsigned int data;
	MX25L_SPI_pid_data_init();	
	for (index = 0; index < 512; index++)
	{
		pid = MX25L_SPI_pid_data_read(index, &data);
		
		printf("READ config: pid = %08X, data = %d\r\n", pid, data);
		
		if (index == 0)
		{
			if ((pid != APP_CONFIG_START_PID) || (data != APP_CONFIG_START_FLAG))
			{
				printf("APP_CONFIG_load: data in wrong format or no data be saved...\r\n");
				return;
			}
		}
	    // 结束	
		if (pid == APP_CONFIG_END_PID||pid == 0xffffffff)
        {   
		    return;
        }
		
		switch (pid)
		{
			case APP_CONFIG_PID_DEV_MODE:
                Discharge.devMode =data;
				break;               
			case APP_CONFIG_PID_RUN_MODE:
                Discharge.mode=data;
				break;
			case APP_CONFIG_PID_UP_MOVE_ENAE:
                Discharge.UpMoveEnable =data;
				break;
			case APP_CONFIG_PID_UP_DRI:
                Discharge.UpDir =data;
				break;         
			case APP_CONFIG_PID_UP_TENSION_POS:
                Discharge.UpTensionPos =data;
				break;              
			case APP_CONFIG_PID_UP_JOG_SPEED:
                Discharge.UpJogSpeed =data;
				break; 
            case APP_CONFIG_PID_UP_ENCODERFDIR:  
                 Encode_handle.EncoderUpDir=data;
                 break;                
			case APP_CONFIG_PID_DOWN_MOVE_ENAE:
                Discharge.DownMoveEnable =data;
				break;
			case APP_CONFIG_PID_DOWN_DRI:
                Discharge.DownDir =data;
				break;         
			case APP_CONFIG_PID_DOWN_TENSION_POS:
                Discharge.DownTensionPos =data;
				break;              
			case APP_CONFIG_PID_DOWN_JOG_SPEED:
                Discharge.DownJogSpeed =data;
				break;   
           case APP_CONFIG_PID_DOWN_ENCODERFDIR:
                 Encode_handle.EncoderDownDir=data;
                break;  
           case APP_CONFIG_PID_LANGUAGE_MODE:
                uiCfg_Language=data;
                break;   
			default:
				break;
		}
	}
}

/*****************************************************************
APP_CONFIG_save:保存配置参数
******************************************************************/
void APP_CONFIG_save(void)
{
	unsigned int index = 0;
	
	MX25L_SPI_pid_data_init();
	
	MX25L_SPI_pid_data_write(index, APP_CONFIG_START_PID, APP_CONFIG_START_FLAG);
    index++;	
	MX25L_SPI_pid_data_write(index, APP_CONFIG_END_PID, index - 1);
	
	MX25L_SPI_pid_data_save();
}
