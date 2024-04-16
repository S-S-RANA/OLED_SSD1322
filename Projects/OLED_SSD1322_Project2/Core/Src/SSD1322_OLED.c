/*
 * SSD1322_OLED.c
 *
 *  Created on: Apr 8, 2024
 *      Author: jprasad
 */
#include "SSD1322_OLED.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern SPI_HandleTypeDef hspi1;

const unsigned char interface = 2;    // 0 = 8-bit parallel (6800 mode) interface; 1 = 8-bit parallel (8080 mode) interface; 2 = 4-wire SPI interface

unsigned char Ascii_1[97][5] = {     // Refer to "Times New Roman" Font Database...
                        //   Basic Characters
    {0x00,0x00,0x00,0x00,0x00},     //   (  0)    - 0x0000 Empty set
    {0x00,0x00,0x4F,0x00,0x00},     //   (  1)  ! - 0x0021 Exclamation Mark
    {0x00,0x07,0x00,0x07,0x00},     //   (  2)  " - 0x0022 Quotation Mark
    {0x14,0x7F,0x14,0x7F,0x14},     //   (  3)  # - 0x0023 Number Sign
    {0x24,0x2A,0x7F,0x2A,0x12},     //   (  4)  $ - 0x0024 Dollar Sign
    {0x23,0x13,0x08,0x64,0x62},     //   (  5)  % - 0x0025 Percent Sign
    {0x36,0x49,0x55,0x22,0x50},     //   (  6)  & - 0x0026 Ampersand
    {0x00,0x05,0x03,0x00,0x00},     //   (  7)  ' - 0x0027 Apostrophe
    {0x00,0x1C,0x22,0x41,0x00},     //   (  8)  ( - 0x0028 Left Parenthesis
    {0x00,0x41,0x22,0x1C,0x00},     //   (  9)  ) - 0x0029 Right Parenthesis
    {0x14,0x08,0x3E,0x08,0x14},     //   ( 10)  * - 0x002A Asterisk
    {0x08,0x08,0x3E,0x08,0x08},     //   ( 11)  + - 0x002B Plus Sign
    {0x00,0x50,0x30,0x00,0x00},     //   ( 12)  , - 0x002C Comma
    {0x08,0x08,0x08,0x08,0x08},     //   ( 13)  - - 0x002D Hyphen-Minus
    {0x00,0x60,0x60,0x00,0x00},     //   ( 14)  . - 0x002E Full Stop
    {0x20,0x10,0x08,0x04,0x02},     //   ( 15)  / - 0x002F Solidus
    {0x3E,0x51,0x49,0x45,0x3E},     //   ( 16)  0 - 0x0030 Digit Zero
    {0x00,0x42,0x7F,0x40,0x00},     //   ( 17)  1 - 0x0031 Digit One
    {0x42,0x61,0x51,0x49,0x46},     //   ( 18)  2 - 0x0032 Digit Two
    {0x21,0x41,0x45,0x4B,0x31},     //   ( 19)  3 - 0x0033 Digit Three
    {0x18,0x14,0x12,0x7F,0x10},     //   ( 20)  4 - 0x0034 Digit Four
    {0x27,0x45,0x45,0x45,0x39},     //   ( 21)  5 - 0x0035 Digit Five
    {0x3C,0x4A,0x49,0x49,0x30},     //   ( 22)  6 - 0x0036 Digit Six
    {0x01,0x71,0x09,0x05,0x03},     //   ( 23)  7 - 0x0037 Digit Seven
    {0x36,0x49,0x49,0x49,0x36},     //   ( 24)  8 - 0x0038 Digit Eight
    {0x06,0x49,0x49,0x29,0x1E},     //   ( 25)  9 - 0x0039 Dight Nine
    {0x00,0x36,0x36,0x00,0x00},     //   ( 26)  : - 0x003A Colon
    {0x00,0x56,0x36,0x00,0x00},     //   ( 27)  ; - 0x003B Semicolon
    {0x08,0x14,0x22,0x41,0x00},     //   ( 28)  < - 0x003C Less-Than Sign
    {0x14,0x14,0x14,0x14,0x14},     //   ( 29)  = - 0x003D Equals Sign
    {0x00,0x41,0x22,0x14,0x08},     //   ( 30)  > - 0x003E Greater-Than Sign
    {0x02,0x01,0x51,0x09,0x06},     //   ( 31)  ? - 0x003F Question Mark
    {0x32,0x49,0x79,0x41,0x3E},     //   ( 32)  @ - 0x0040 Commercial At
    {0x7E,0x11,0x11,0x11,0x7E},     //   ( 33)  A - 0x0041 Latin Capital Letter A
    {0x7F,0x49,0x49,0x49,0x36},     //   ( 34)  B - 0x0042 Latin Capital Letter B
    {0x3E,0x41,0x41,0x41,0x22},     //   ( 35)  C - 0x0043 Latin Capital Letter C
    {0x7F,0x41,0x41,0x22,0x1C},     //   ( 36)  D - 0x0044 Latin Capital Letter D
    {0x7F,0x49,0x49,0x49,0x41},     //   ( 37)  E - 0x0045 Latin Capital Letter E
    {0x7F,0x09,0x09,0x09,0x01},     //   ( 38)  F - 0x0046 Latin Capital Letter F
    {0x3E,0x41,0x49,0x49,0x7A},     //   ( 39)  G - 0x0047 Latin Capital Letter G
    {0x7F,0x08,0x08,0x08,0x7F},     //   ( 40)  H - 0x0048 Latin Capital Letter H
    {0x00,0x41,0x7F,0x41,0x00},     //   ( 41)  I - 0x0049 Latin Capital Letter I
    {0x20,0x40,0x41,0x3F,0x01},     //   ( 42)  J - 0x004A Latin Capital Letter J
    {0x7F,0x08,0x14,0x22,0x41},     //   ( 43)  K - 0x004B Latin Capital Letter K
    {0x7F,0x40,0x40,0x40,0x40},     //   ( 44)  L - 0x004C Latin Capital Letter L
    {0x7F,0x02,0x0C,0x02,0x7F},     //   ( 45)  M - 0x004D Latin Capital Letter M
    {0x7F,0x04,0x08,0x10,0x7F},     //   ( 46)  N - 0x004E Latin Capital Letter N
    {0x3E,0x41,0x41,0x41,0x3E},     //   ( 47)  O - 0x004F Latin Capital Letter O
    {0x7F,0x09,0x09,0x09,0x06},     //   ( 48)  P - 0x0050 Latin Capital Letter P
    {0x3E,0x41,0x51,0x21,0x5E},     //   ( 49)  Q - 0x0051 Latin Capital Letter Q
    {0x7F,0x09,0x19,0x29,0x46},     //   ( 50)  R - 0x0052 Latin Capital Letter R
    {0x46,0x49,0x49,0x49,0x31},     //   ( 51)  S - 0x0053 Latin Capital Letter S
    {0x01,0x01,0x7F,0x01,0x01},     //   ( 52)  T - 0x0054 Latin Capital Letter T
    {0x3F,0x40,0x40,0x40,0x3F},     //   ( 53)  U - 0x0055 Latin Capital Letter U
    {0x1F,0x20,0x40,0x20,0x1F},     //   ( 54)  V - 0x0056 Latin Capital Letter V
    {0x3F,0x40,0x38,0x40,0x3F},     //   ( 55)  W - 0x0057 Latin Capital Letter W
    {0x63,0x14,0x08,0x14,0x63},     //   ( 56)  X - 0x0058 Latin Capital Letter X
    {0x07,0x08,0x70,0x08,0x07},     //   ( 57)  Y - 0x0059 Latin Capital Letter Y
    {0x61,0x51,0x49,0x45,0x43},     //   ( 58)  Z - 0x005A Latin Capital Letter Z
    {0x00,0x7F,0x41,0x41,0x00},     //   ( 59)  [ - 0x005B Left Square Bracket
    {0x02,0x04,0x08,0x10,0x20},     //   ( 60)  \ - 0x005C Reverse Solidus
    {0x00,0x41,0x41,0x7F,0x00},     //   ( 61)  ] - 0x005D Right Square Bracket
    {0x04,0x02,0x01,0x02,0x04},     //   ( 62)  ^ - 0x005E Circumflex Accent
    {0x40,0x40,0x40,0x40,0x40},     //   ( 63)  _ - 0x005F Low Line
    {0x01,0x02,0x04,0x00,0x00},     //   ( 64)  ` - 0x0060 Grave Accent
    {0x20,0x54,0x54,0x54,0x78},     //   ( 65)  a - 0x0061 Latin Small Letter A
    {0x7F,0x48,0x44,0x44,0x38},     //   ( 66)  b - 0x0062 Latin Small Letter B
    {0x38,0x44,0x44,0x44,0x20},     //   ( 67)  c - 0x0063 Latin Small Letter C
    {0x38,0x44,0x44,0x48,0x7F},     //   ( 68)  d - 0x0064 Latin Small Letter D
    {0x38,0x54,0x54,0x54,0x18},     //   ( 69)  e - 0x0065 Latin Small Letter E
    {0x08,0x7E,0x09,0x01,0x02},     //   ( 70)  f - 0x0066 Latin Small Letter F
    {0x06,0x49,0x49,0x49,0x3F},     //   ( 71)  g - 0x0067 Latin Small Letter G
    {0x7F,0x08,0x04,0x04,0x78},     //   ( 72)  h - 0x0068 Latin Small Letter H
    {0x00,0x44,0x7D,0x40,0x00},     //   ( 73)  i - 0x0069 Latin Small Letter I
    {0x20,0x40,0x44,0x3D,0x00},     //   ( 74)  j - 0x006A Latin Small Letter J
    {0x7F,0x10,0x28,0x44,0x00},     //   ( 75)  k - 0x006B Latin Small Letter K
    {0x00,0x41,0x7F,0x40,0x00},     //   ( 76)  l - 0x006C Latin Small Letter L
    {0x7C,0x04,0x18,0x04,0x7C},     //   ( 77)  m - 0x006D Latin Small Letter M
    {0x7C,0x08,0x04,0x04,0x78},     //   ( 78)  n - 0x006E Latin Small Letter N
    {0x38,0x44,0x44,0x44,0x38},     //   ( 79)  o - 0x006F Latin Small Letter O
    {0x7C,0x14,0x14,0x14,0x08},     //   ( 80)  p - 0x0070 Latin Small Letter P
    {0x08,0x14,0x14,0x18,0x7C},     //   ( 81)  q - 0x0071 Latin Small Letter Q
    {0x7C,0x08,0x04,0x04,0x08},     //   ( 82)  r - 0x0072 Latin Small Letter R
    {0x48,0x54,0x54,0x54,0x20},     //   ( 83)  s - 0x0073 Latin Small Letter S
    {0x04,0x3F,0x44,0x40,0x20},     //   ( 84)  t - 0x0074 Latin Small Letter T
    {0x3C,0x40,0x40,0x20,0x7C},     //   ( 85)  u - 0x0075 Latin Small Letter U
    {0x1C,0x20,0x40,0x20,0x1C},     //   ( 86)  v - 0x0076 Latin Small Letter V
    {0x3C,0x40,0x30,0x40,0x3C},     //   ( 87)  w - 0x0077 Latin Small Letter W
    {0x44,0x28,0x10,0x28,0x44},     //   ( 88)  x - 0x0078 Latin Small Letter X
    {0x0C,0x50,0x50,0x50,0x3C},     //   ( 89)  y - 0x0079 Latin Small Letter Y
    {0x44,0x64,0x54,0x4C,0x44},     //   ( 90)  z - 0x007A Latin Small Letter Z
    {0x00,0x08,0x36,0x41,0x00},     //   ( 91)  { - 0x007B Left Curly Bracket
    {0x00,0x00,0x7F,0x00,0x00},     //   ( 92)  | - 0x007C Vertical Line
    {0x00,0x41,0x36,0x08,0x00},     //   ( 93)  } - 0x007D Right Curly Bracket
    {0x02,0x01,0x02,0x04,0x02},     //   ( 94)  ~ - 0x007E Tilde
    {0x08,0x0C,0x0E,0x0C,0x08},     //   ( 95)  upward facing triangle/arrow
    {0x08,0x18,0x38,0x18,0x08},     //   ( 96)  downward facing triangle/arrow
};
/*===============================*/
/*======= FONT TABLE 5x8 ========*/
/*============= END =============*/
/*===============================*/


unsigned char Ascii_2[95][4] = {
			{0x00,0x00,0x00,0x00}, // <space>
		    {0x00,0x17,0x00,0x00}, // !
		    {0x03,0x00,0x03,0x00}, // "
		    {0x1f,0x0a,0x1f,0x00}, // #
		    {0x16,0x1f,0x0d,0x00}, // $
		    {0x19,0x04,0x13,0x00}, // %
		    {0x0a,0x15,0x1a,0x00}, // &
		    {0x00,0x03,0x00,0x00}, // '
		    {0x00,0x0e,0x11,0x00}, // (
		    {0x11,0x0e,0x00,0x00}, // )
		    {0x0a,0x04,0x0a,0x00}, // *
		    {0x04,0x0e,0x04,0x00}, // +
		    {0x10,0x08,0x00,0x00}, // ,
		    {0x04,0x04,0x04,0x00},  // -
		    {0x00,0x10,0x00,0x00}, // .
		    {0x18,0x04,0x03,0x00},  // /
		    {0x1f,0x11,0x1f,0x00}, // 0
		    {0x12,0x1f,0x10,0x00}, // 1
		    {0x1a,0x19,0x16,0x00}, // 2
		    {0x11,0x15,0x0a,0x00}, // 3
		    {0x07,0x04,0x1f,0x00}, // 4
		    {0x17,0x15,0x0d,0x00}, // 5
		    {0x0e,0x15,0x09,0x00}, // 6
		    {0x19,0x05,0x03,0x00}, // 7
		    {0x1f,0x15,0x1f,0x00}, // 8
		    {0x12,0x15,0x0e,0x00}, // 9
		    {0x00,0x0a,0x00,0x00}, // :
		    {0x10,0x0a,0x00,0x00}, // ;
		    {0x04,0x0a,0x11,0x00}, // <
		    {0x0a,0x0a,0x0a,0x00}, // =
		    {0x11,0x0a,0x04,0x00}, // >
		    {0x02,0x19,0x06,0x00}, // ?
		    {0x0e,0x15,0x16,0x00}, // @
		    {0x1e,0x05,0x1e,0x00}, // A
		    {0x1f,0x15,0x0a,0x00}, // B
		    {0x0e,0x11,0x0a,0x00}, // C
		    {0x1f,0x11,0x0e,0x00}, // D
		    {0x1f,0x15,0x11,0x00}, // E
		    {0x1f,0x05,0x01,0x00}, // F
		    {0x0e,0x11,0x0d,0x00}, // G
		    {0x1f,0x04,0x1f,0x00}, // H
		    {0x11,0x1f,0x11,0x00}, // I
		    {0x11,0x1f,0x01,0x00}, // J
		    {0x1f,0x04,0x1b,0x00}, // K
		    {0x1f,0x10,0x10,0x00}, // L
		    {0x1f,0x02,0x1f,0x00}, // M
		    {0x1f,0x0e,0x1f,0x00}, // N
		    {0x0e,0x11,0x0e,0x00}, // O
		    {0x1f,0x05,0x02,0x00}, // P
		    {0x0e,0x11,0x2e,0x00}, // Q
		    {0x1f,0x05,0x1a,0x00}, // R
		    {0x12,0x15,0x09,0x00}, // S
		    {0x01,0x1f,0x01,0x00}, // T
		    {0x0f,0x10,0x0f,0x00}, // U
		    {0x07,0x18,0x07,0x00},  // V
		    {0x0f,0x1c,0x0f,0x00}, // W
		    {0x1b,0x04,0x1b,0x00}, // X
		    {0x03,0x1c,0x03,0x00}, // Y
		    {0x19,0x15,0x13,0x00}, // Z
		    {0x00,0x1f,0x11,0x00}, // [
		    {0x03,0x04,0x18,0x00}, // <backslash>
		    {0x11,0x1f,0x00,0x00}, // ]
		    {0x02,0x01,0x02,0x00}, // ^
		    {0x10,0x10,0x10,0x00}, // _
		    {0x00,0x01,0x02,0x00}, // `
		    {0x0c,0x12,0x1c,0x00}, // a
		    {0x1f,0x12,0x0c,0x00}, // b
		    {0x0c,0x12,0x12,0x00}, // c
		    {0x0c,0x12,0x1f,0x00}, // d
		    {0x0c,0x1a,0x14,0x00}, // e
		    {0x04,0x1f,0x05,0x00}, // f
		    {0x24,0x2a,0x1c,0x00}, // g
		    {0x1f,0x02,0x1c,0x00}, // h
		    {0x00,0x1d,0x00,0x00}, // i
		    {0x20,0x1d,0x00,0x00}, // j
		    {0x1f,0x08,0x14,0x00}, // k
		    {0x00,0x1f,0x00,0x00}, // l
		    {0x1e,0x06,0x1e,0x00}, // m
		    {0x1c,0x02,0x1c,0x00}, // n
		    {0x0c,0x12,0x0c,0x00}, // o
		    {0x3e,0x12,0x0c,0x00}, // p
		    {0x0c,0x12,0x3e,0x00}, // q
		    {0x1e,0x04,0x02,0x00}, // r
		    {0x14,0x0a,0x00,0x00}, // s
		    {0x02,0x1f,0x02,0x00}, // t
		    {0x0e,0x10,0x0e,0x00}, // u
		    {0x06,0x18,0x06,0x00}, // v
		    {0x0e,0x18,0x0e,0x00}, // w
		    {0x12,0x0c,0x12,0x00}, // x
		    {0x2e,0x28,0x1e,0x00}, // y
		    {0x1a,0x1e,0x16,0x00}, // z
		    {0x04,0x1f,0x11,0x00}, // {
		    {0x00,0x1b,0x00,0x00}, // |
		    {0x11,0x1f,0x04,0x00}, // }
		    {0x06,0x02,0x03,0x00} // ~
};

void OLED_Command_12864RGB(unsigned char c)        // send command to OLED
{

				  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET); // Set CS_PIN LOW
				  HAL_GPIO_WritePin(GPIOD, RS_Pin, GPIO_PIN_RESET); // Set RS_PIN LOW

				  HAL_SPI_Transmit(&hspi1, &c, 1, 200);

//				  if(c&0x01)
//					  HAL_GPIO_WritePin(GPIOC, D0_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D0_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x02)
//					  HAL_GPIO_WritePin(GPIOC, D1_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D1_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x04)
//					  HAL_GPIO_WritePin(GPIOC, D2_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D2_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x08)
//					  HAL_GPIO_WritePin(GPIOC, D3_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D3_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x10)
//					  HAL_GPIO_WritePin(GPIOC, D4_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D4_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x20)
//					  HAL_GPIO_WritePin(GPIOC, D5_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D5_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x40)
//					  HAL_GPIO_WritePin(GPIOC, D6_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D6_Pin, GPIO_PIN_RESET);
//
//
//				  if(c&0x80)
//					  HAL_GPIO_WritePin(GPIOC, D7_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D7_Pin, GPIO_PIN_RESET);


				  HAL_GPIO_WritePin(GPIOD, WR_Pin, GPIO_PIN_RESET); // Set RW_PIN LOW
				  HAL_GPIO_WritePin(GPIOD, RD_Pin, GPIO_PIN_SET);    // Set E_PIN HIGH
				  HAL_GPIO_WritePin(GPIOD, RD_Pin, GPIO_PIN_RESET);  // Set E_PIN LOW
				  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);   // Set CS_PIN HIGH



}

void OLED_Data_12864RGB(unsigned char d)        // send data to OLED
{

				  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET); // Set CS_PIN LOW
				  HAL_GPIO_WritePin(GPIOD, RS_Pin, GPIO_PIN_SET); // Set RS_PIN

				  HAL_SPI_Transmit(&hspi1, &d, 1, 200);

//				  if(d&0x01)
//					  HAL_GPIO_WritePin(GPIOC, D0_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D0_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x02)
//					  HAL_GPIO_WritePin(GPIOC, D1_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D1_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x04)
//					  HAL_GPIO_WritePin(GPIOC, D2_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D2_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x08)
//					  HAL_GPIO_WritePin(GPIOC, D3_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D3_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x10)
//					  HAL_GPIO_WritePin(GPIOC, D4_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D4_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x20)
//					  HAL_GPIO_WritePin(GPIOC, D5_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D5_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x40)
//					  HAL_GPIO_WritePin(GPIOC, D6_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D6_Pin, GPIO_PIN_RESET);
//
//
//				  if(d&0x80)
//					  HAL_GPIO_WritePin(GPIOC, D7_Pin,GPIO_PIN_SET);
//				  else
//					  HAL_GPIO_WritePin(GPIOC, D7_Pin, GPIO_PIN_RESET);




				  HAL_GPIO_WritePin(GPIOD, WR_Pin, GPIO_PIN_RESET); // Set RW_PIN LOW
				  HAL_GPIO_WritePin(GPIOD, RD_Pin, GPIO_PIN_SET);  // Set E_PIN
				  HAL_GPIO_WritePin(GPIOD, RD_Pin, GPIO_PIN_RESET);  // Set E_PIN LOW
				  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET); // Set CS_PIN

}

void OLED_SetColumnAddress_12864RGB(unsigned char x_start, unsigned char x_end)    // set column address start + end
{
   OLED_Command_12864RGB(0x15);
   OLED_Data_12864RGB(x_start);
   OLED_Data_12864RGB(x_end);
}

void OLED_SetRowAddress_12864RGB(unsigned char y_start, unsigned char y_end)    // set row address start + end
{
   OLED_Command_12864RGB(0x75);
   OLED_Data_12864RGB(y_start);
   OLED_Data_12864RGB(y_end);
}

void OLED_WriteMemoryStart_12864RGB(void)    // write to RAM command
{
    OLED_Command_12864RGB(0x5C);
}

void OLED_Pixel_12864RGB(unsigned long color)    // write one pixel of a given color
{
        OLED_Data_12864RGB((color>>16));
        OLED_Data_12864RGB((color>>8));
        OLED_Data_12864RGB(color);
}

void OLED_FillScreen_12864RGB(unsigned long color)    // fill screen with a given color
{
   unsigned int i,j;
   OLED_SetColumnAddress_12864RGB(0x00, 0x7F);
   OLED_SetRowAddress_12864RGB(0x00, 0x40);
   OLED_WriteMemoryStart_12864RGB();
   for(i=0;i<64;i++)
   {
      for(j=0;j<128;j++)
      {
    	  OLED_Pixel_12864RGB(color);
      }
   }
}

/*===============================*/
/*===== LOW LEVEL FUNCTIONS =====*/
/*============= END =============*/
/*===============================*/

void OLED_Text_12864RGB(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor)  // function to show text
{
    int i;
    int count;
    unsigned char mask = 0x01;

    for(i=0;i<8;i++)     //each character is 8 px tall
    {
        OLED_SetColumnAddress_12864RGB(x_pos, 0x77);
        OLED_SetRowAddress_12864RGB(y_pos, 0x7F);
        OLED_WriteMemoryStart_12864RGB();
        for (count=0;count<5;count++)    //each character is 5px wide
        {
            if((Ascii_1[letter][count] & mask) == mask)
                OLED_Pixel_12864RGB(textColor);
            else
                OLED_Pixel_12864RGB(backgroundColor);
        }
        y_pos++;
        mask = mask << 1;
   }
}void OLED_Text_4x6(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor)  // function to show text
{
    int i;
    int count;
    unsigned char mask = 0x01;

    for(i=0;i<6;i++)     //each character is 8 px tall
    {
        OLED_SetColumnAddress_12864RGB(x_pos, 0x77);
        OLED_SetRowAddress_12864RGB(y_pos, 0x7F);
        OLED_WriteMemoryStart_12864RGB();
        for (count=0;count<4;count++)    //each character is 4px wide
        {
            if((Ascii_2[letter][count] & mask) == mask)
                OLED_Pixel_12864RGB(textColor);
            else
                OLED_Pixel_12864RGB(backgroundColor);
        }
        y_pos++;
        mask = mask << 1;
   }
}

void OLED_Text2x_12864RGB(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor)  // function to show text (2x size)
{
    int i;
    int count;
    unsigned char mask = 0x01;		//0x80;

    for(i=1;i<=16;i++)     // each character is 16 pixels tall
    {
        OLED_SetColumnAddress_12864RGB(x_pos, 0x7F);
        OLED_SetRowAddress_12864RGB(y_pos, 0x40);
        OLED_WriteMemoryStart_12864RGB();
        for (count=0;count<10;count++)    // each character is 10 pixels wide
        {
            if((Ascii_1[letter][(count/2)] & mask) == mask)
                OLED_Pixel_12864RGB(textColor);
            else
                OLED_Pixel_12864RGB(backgroundColor);
        }
        y_pos++;
        if((i%2)==0)
        {
            mask = mask << 1;		//mask = mask >> 1;
        }
   }
}
int printString( char name[8], int xpos, int ypos)
{

    int ascii_values[8] = {0};
    for (int i = 0; name[i] != '\0'; i++) {
        ascii_values[i] = name[i];
    }
	for(int i = 0;i < 10;i++)
	{
		OLED_Text_12864RGB(xpos+i*8, ypos, ascii_values[i]-32, WHITE, BLACK);

	}
	return 0;
}



void ImageDisplay_12864(unsigned char *image)

{

  unsigned int i, l, n, buff;



//Set_Remap_12864(0x04,0x11);         //To flip 180 degrees

  OLED_SetColumnAddress_12864RGB(0x1C,0x5B);//(0x1C,0x5B);

  OLED_SetRowAddress_12864RGB(0x00,0x3F);

  OLED_WriteMemoryStart_12864RGB();

  for(i=0;i<1024;i++){

    buff = *image;

    n = 7;

    for(l=0;l<8;l++){

      if((buff >> n) & (0x80 == 0x80)){

    	  OLED_Data_12864RGB(0xFF);

        }

      else{

    	  OLED_Data_12864RGB(0x00);

        }

      n--;

    }

      image++;

  }

}


/*********************************/
/******** INITIALIZATION *********/
/************ START **************/
/*********************************/

//void OLED_Init_12864RGB(void)      //OLED initialization

void OLED_Init_12864RGB(void)      //OLED initialization
{

 HAL_GPIO_WritePin(GPIOB, RES_Pin, GPIO_PIN_RESET); // Set RES_PIN LOW
 Delay(500);
 HAL_GPIO_WritePin(GPIOB, RES_Pin, GPIO_PIN_SET); // Set RES_PIN LOW
 Delay(500);

    //Set_Sleep_Mode_12864(0xAF);                          // Display OFF
  OLED_Command_12864RGB(0xAF);
  //Set_Command_Lock_12864(0x12);                        // Unlock commands for OLED
  OLED_Command_12864RGB(0xFD);
  OLED_Data_12864RGB(0x12);
  //Set_Display_Clock__Oscillator_Frequency_12864(0x91); // Set Display Clock / Oscillator Frequency
  OLED_Command_12864RGB(0xB3);
  OLED_Data_12864RGB(0x91);

  //Set_Multiplex_Ratio_12864(0x3F);                     // Set Multiplex Ratio
  OLED_Command_12864RGB(0xCA);
  OLED_Data_12864RGB(0x3F);

  //Set_Display_Offset_12864(0x00);                      // Set Display Offset
  OLED_Command_12864RGB(0xA2);
  OLED_Data_12864RGB(0x40);

  //Function_Selection_12864(0x01);                      // Function Selection
  OLED_Command_12864RGB(0xAB);
  OLED_Data_12864RGB(0x01);

  //Set_Display_Start_Line_12864(0x00);                  // Set Display Start Line
  OLED_Command_12864RGB(0xA1);
  OLED_Data_12864RGB(0x00);

  //Set_Remap_12864(0x16, 0x11);                         // Set Remap. (A[1]) & (A[4]) can be adjusted to flip display orientation
  OLED_Command_12864RGB(0xA0);
  OLED_Data_12864RGB(0x16);
  OLED_Data_12864RGB(0x11);
  //Master_Contrast_Control_12864(0x0F);                 // Master Contrast Control
  OLED_Command_12864RGB(0xC7);
  OLED_Data_12864RGB(0x0F);

  //Set_Contrast_Control_12864(0x9F);                    // Set Contrast Control
  OLED_Command_12864RGB(0xC1);
  OLED_Data_12864RGB(0x9F);

  //Set_Phase_Length_12864(0x72);
  OLED_Command_12864RGB(0xB1);
  OLED_Data_12864RGB(0x72);
  // Set Phase Length
  //Set_Precharge_Voltage_12864(0x1F);                   // Set Precharge Voltage
  OLED_Command_12864RGB(0xBB);
  OLED_Data_12864RGB(0x1F);

  //Enable_External_VSL(0xA0, 0xFD);                     // Enable External VSL
  OLED_Command_12864RGB(0xB4);
  OLED_Data_12864RGB(0xA0);
  OLED_Data_12864RGB(0xFD);

  //Set_VCOMH_Voltage_12864(0x04);                       // Set VCOMH Voltage
  OLED_Command_12864RGB(0xBE);
  OLED_Data_12864RGB(0x04);

  //Display_Mode_12864(0xA6);                            // Set Normal Display
  OLED_Command_12864RGB(0xA6);

  //Exit_Partial_Display_12864();                        // Exit Partial Display Mode
  OLED_Command_12864RGB(0xA9);
  //Display_Enhancement_12864(0xA2);         // Display Enhancement
  OLED_Command_12864RGB(0xD1);
  OLED_Data_12864RGB(0xA2);
  OLED_Data_12864RGB(0x20);

  //Set_GPIO_12864(0x00);                    // Set GPIO
  OLED_Command_12864RGB(0xB5);
  OLED_Data_12864RGB(0x00);

  //Default_Grayscale_Command_12864();       // Default Linear Grayscale Table
  OLED_Command_12864RGB(0xB9);
  //Set_Second_Precharge_Period_12864(0x08); // Set Second Precharge Period
  OLED_Command_12864RGB(0xB6);
  OLED_Data_12864RGB(0x08);

  //Set_Sleep_Mode_12864(0xAF); // Display ON
  OLED_Command_12864RGB(0xAF);
}

/*===============================*/
/*======= INITIALIZATION ========*/
/*============= END =============*/
/*===============================*/

void setup()                                       // for Arduino, runs first at power on
{


	  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOB, BS0_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOB, BS1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD, WR_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD, RD_Pin, GPIO_PIN_SET);

}




void Delay(int miliseconds){
	for (int i = miliseconds; i < miliseconds; i--){
		for (int j = 0; j > 24000; j++);
	}
}
