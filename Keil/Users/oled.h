/*������ͷ�ļ�*/
#ifndef __OLED_H_
#define __OLED_H_

#include "i2c.h"

extern unsigned char bmp1[];

void oled_full(uint8_t data); //ȫ�����
void oled_init(void); //��ʼ��
void oled_show_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);//���ֽ�
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize); //����ַ���
void oled_show_chinese(uint8_t x,uint8_t y,uint8_t no); //�������
void oled_show_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[]);//���ͼ��
void oled_clear(void); //����

void oled_show_info(int Cur_Dis);	//��Ļ��ʾ for DT35
void oled_show_init(void);
#endif  
