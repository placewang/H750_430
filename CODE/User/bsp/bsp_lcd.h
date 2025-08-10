#ifndef _BSP_LCD_H_
#define _BSP_LCD_H_

#define LCD_WIDTH           480
#define LCD_HEIGHT          272

//#define LCD_WIDTH           800
//#define LCD_HEIGHT          480

#define LCD_BITS_PER_PIXEL  16 //24
#define LCD_BUF_SIZE        (LCD_WIDTH * LCD_HEIGHT * LCD_BITS_PER_PIXEL / 8)
#define LCD_PIXEL_FORMAT    RTGRAPHIC_PIXEL_FORMAT_RGB565
#define LCD_PIXSIZE    2  


/* graphic deice */
enum
{
    RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
    RTGRAPHIC_PIXEL_FORMAT_GRAY4,
    RTGRAPHIC_PIXEL_FORMAT_GRAY16,
    RTGRAPHIC_PIXEL_FORMAT_RGB332,
    RTGRAPHIC_PIXEL_FORMAT_RGB444,
    RTGRAPHIC_PIXEL_FORMAT_RGB565,
    RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    RTGRAPHIC_PIXEL_FORMAT_BGR565 = RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    RTGRAPHIC_PIXEL_FORMAT_RGB666,
    RTGRAPHIC_PIXEL_FORMAT_RGB888,
    RTGRAPHIC_PIXEL_FORMAT_ARGB888,
    RTGRAPHIC_PIXEL_FORMAT_ABGR888,
    RTGRAPHIC_PIXEL_FORMAT_RESERVED,
};


//画笔颜色
#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000	  
#define COLOR_BLUE		0x001F  
#define COLOR_BRED		0XF81F
#define COLOR_GRED		0XFFE0
#define COLOR_GBLUE		0X07FF
#define COLOR_RED		0xF800
#define COLOR_MAGENTA	0xF81F
#define COLOR_GREEN		0x07E0
#define COLOR_CYAN		0x7FFF
#define COLOR_YELLOW	0xFFE0
#define COLOR_BROWN		0XBC40 //棕色
#define COLOR_BRRED		0XFC07 //棕红色
#define COLOR_GRAY		0X8430 //灰色


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 	 0x001F  
#define BRED             0XF81F
#define GRED 						 0XFFE0
#define GBLUE			 			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 		 0XBC40 //棕色
#define BRRED 			 		 0XFC07 //棕红色
#define GRAY  					 0X8430 //灰色

#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888格式
#define LCD_PIXFORMAT_RGB888 		0X01			//ARGB8888格式
#define LCD_PIXFORMAT_RGB565 		0X02 			//ARGB8888格式
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB8888格式
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB8888格式
#define LCD_PIXFORMAT_L8			0X05			//ARGB8888格式
#define LCD_PIXFORMAT_AL44			0X06			//ARGB8888格式
#define LCD_PIXFORMAT_AL88 			0X07			//ARGB8888格式

#define _LCD_LockScreenBrightness 4000
#define _LCD_ScreenBrightness     12000
void LTDC_Color_Fill(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short *color_p);
void LTDC_Draw_Point(unsigned short x, unsigned short y, unsigned short color);

void BSP_LCD_backlight_Brightness(unsigned short DutyCycle);

void BSP_LCD_init(void);
void BSP_LCD_loop_1ms(void);

void BSP_LCD_set_fb_startadress_IT(unsigned int fb);
int BSP_LCD_if_fb_reload_cmpt(void);
#endif

