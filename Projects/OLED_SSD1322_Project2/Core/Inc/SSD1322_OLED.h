/*
 * SSD1322_OLED.h
 *
 *  Created on: Apr 8, 2024
 *      Author: jprasad
 */

#ifndef INC_SSD1322_OLED_H_
#define INC_SSD1322_OLED_H_

#define  WHITE  0xFFFFFF
#define  BLACK  0x000000

void OLED_Command_12864RGB(unsigned char c);
void OLED_Data_12864RGB(unsigned char d);
void OLED_SetColumnAddress_12864RGB(unsigned char x_start, unsigned char x_end);
void OLED_SetRowAddress_12864RGB(unsigned char y_start, unsigned char y_end);
void OLED_WriteMemoryStart_12864RGB(void);
void OLED_Pixel_12864RGB(unsigned long color);
void OLED_FillScreen_12864RGB(unsigned long color);
void OLED_Text_12864RGB(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor);
void OLED_Text2x_12864RGB(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor);  // function to show text (2x size)
int printString( char name[8], int xpos, int ypos);
void ImageDisplay_12864(unsigned char *image);
void OLED_Init_12864RGB(void);      //OLED initialization
void setup();
void Delay(int miliseconds);


#endif /* INC_SSD1322_OLED_H_ */
