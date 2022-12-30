/*这里是头文件*/
#ifndef __OLED_H_
#define __OLED_H_

#include "i2c.h"

extern unsigned char bmp1[];

void oled_full(uint8_t data); //全屏填充
void oled_init(void); //初始化
void oled_show_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);//单字节
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize); //输出字符串
void oled_show_chinese(uint8_t x,uint8_t y,uint8_t no); //输出汉字
void oled_show_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[]);//输出图像
void oled_clear(void); //清屏

void oled_show_info(int Cur_Dis);	//屏幕显示 for DT35
void oled_show_init(void);
#endif  
