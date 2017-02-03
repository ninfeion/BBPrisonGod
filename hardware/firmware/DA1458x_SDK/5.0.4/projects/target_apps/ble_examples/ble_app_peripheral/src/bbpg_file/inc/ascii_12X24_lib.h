//#ifndef _ACSII_12X24_LIB_H_
//#define _ACSII_12X24_LIB_H_

// ------------------  ASCII��ģ�����ݱ� ------------------------ //
// �����0x20~0x7e                                                //
// ����ȡģ�¸�λ                                                 //
// -------------------------------------------------------------- //
//extern unsigned char ASCII_LIB[];

unsigned char ASCII_LIB[] =                                             // ASCII
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // - -
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xFC,0xFE,0xFC,0x00,0x00,0x00,0x00,  // -!-
	0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0x03,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x30,0x79,0x30,0x00,0x00,0x00,0x00,

	0x00,0x00,0x1C,0xFC,0x1C,0x00,0x00,0x1C,0xFC,0x1C,0x00,0x00,  // -"-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0xE0,0xE0,0x00,0x00,0x00,0xF0,0xF0,0x80,0x80,0x00,  // -#-
	0x82,0x82,0xFF,0xFF,0x41,0x41,0x41,0xFF,0xFF,0x20,0x20,0x00,
	0x00,0x00,0x1F,0x1F,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,

	0xC0,0xE0,0x30,0x10,0x10,0xFC,0xFC,0x10,0x10,0x30,0xE0,0xC0,  // -$-
	0x03,0x07,0x0C,0x08,0x18,0xFF,0xFF,0x30,0x30,0x60,0xC0,0x80,
	0x07,0x0F,0x18,0x10,0x10,0x7F,0x7F,0x10,0x10,0x18,0x0F,0x07,

	0xF0,0xF8,0x04,0x04,0xF8,0xF0,0x00,0x00,0xC0,0x70,0x18,0x00,  // -%-
	0x01,0x03,0x04,0x84,0xE3,0x39,0x8E,0x43,0x41,0x80,0x00,0x00,
	0x30,0x1C,0x07,0x01,0x00,0x1F,0x3F,0x40,0x40,0x3F,0x1F,0x00,

	0x00,0xF0,0x88,0x04,0x04,0x84,0x78,0x00,0x00,0x00,0x00,0x00,  // -&-
	0xE0,0xF8,0x07,0x06,0x1D,0x70,0xC0,0x80,0x00,0x40,0xC0,0x40,
	0x0F,0x1F,0x30,0x20,0x20,0x30,0x11,0x1B,0x0E,0x1E,0x31,0x20,

	0x00,0x00,0x00,0x00,0x00,0x3C,0xFC,0x3C,0x00,0x00,0x00,0x00,  // -'-
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0x78,0x0C,0x02,0x01,0x00,  // -(-
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1E,0x30,0x40,0x80,0x00,

	0x00,0x01,0x02,0x0C,0x78,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,  // -)-
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x80,0x40,0x30,0x1E,0x07,0x01,0x00,0x00,0x00,0x00,0x00,

	0x80,0x80,0x00,0x00,0x60,0xF0,0x60,0x00,0x00,0x80,0x80,0x00,  // -*-
	0x81,0xC3,0xC3,0x24,0x00,0xC3,0x00,0x24,0xC3,0xC3,0x81,0x00,
	0x01,0x01,0x00,0x00,0x06,0x0F,0x06,0x00,0x00,0x01,0x01,0x00,

	0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,  // -+-
	0x00,0x18,0x18,0x18,0x18,0xFF,0xFF,0x18,0x18,0x18,0x18,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -,-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x8C,0xDE,0x7E,0x3C,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // ---
	0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -.-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x70,0x1C,0x06,0x00,  // -/-
	0x00,0x00,0x00,0xC0,0x70,0x1C,0x07,0x01,0x00,0x00,0x00,0x00,
	0x30,0x1C,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    0x00,0xC0,0xF0,0x38,0x0C,0x04,0x0C,0x38,0xF0,0xC0,0x00,0x00,  // -0- bbf
	0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,
	0x00,0x03,0x0F,0x1C,0x30,0x20,0x30,0x1C,0x0F,0x03,0x00,0x00,

	0x00,0x30,0x30,0x30,0xF8,0xF8,0xFC,0xFC,0x00,0x00,0x00,0x00,  // -1-
	0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,

	0x00,0xF0,0xF8,0xFC,0x0C,0x1C,0xFC,0xFC,0xFC,0xF8,0xF0,0x00,  // -2-
	0x00,0x00,0xC0,0xF0,0xFC,0xFF,0xFF,0xFF,0x3F,0x0F,0x03,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x33,0x30,0x30,0x30,0x30,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x0C,0x0C,0xFC,0xFC,0xF8,0xF0,0x00,  // -3-
	0x00,0x00,0x00,0x00,0x1C,0x1C,0x3E,0xFF,0xFB,0xF3,0xE1,0x00,
	0x00,0x1F,0x3F,0x3F,0x3F,0x30,0x38,0x3F,0x1F,0x1F,0x0F,0x00,

	0x00,0x00,0x00,0x00,0xC0,0xF0,0xFC,0xFC,0xFC,0xFC,0x00,0x00,  // -4-
	0x00,0xF0,0xFC,0xBF,0x8F,0x83,0xFF,0xFF,0xFF,0xFF,0x80,0x00,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x3F,0x3F,0x3F,0x03,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00,  // -5-
	0x00,0x8F,0x8F,0x8F,0x8F,0x06,0x07,0xFF,0xFF,0xFE,0xFC,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x1C,0x0C,0x7C,0x7C,0x78,0x70,0x00,  // -6-
	0x00,0xFF,0xFF,0xFF,0xFF,0x04,0x06,0xFE,0xFE,0xFC,0xF8,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x1C,0x1C,0x1C,0x1C,0x1C,0x9C,0xFC,0xFC,0xFC,0xFC,0x00,  // -7-
	0x00,0x00,0x00,0x80,0xF0,0xFC,0xFF,0xFF,0x1F,0x03,0x00,0x00,
	0x00,0x00,0x3C,0x3F,0x3F,0x3F,0x07,0x00,0x00,0x00,0x00,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x0C,0x0C,0xFC,0xFC,0xF8,0xF0,0x00,  // -8-
	0x00,0xF1,0xFB,0xFF,0xFF,0x0E,0x0E,0xFF,0xFF,0xFB,0xF1,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x38,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x0C,0x0C,0xFC,0xFC,0xF8,0xF0,0x00,  // -9- bbf
	0x00,0x1F,0x3F,0x7F,0x7F,0x70,0x30,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x0E,0x1E,0x3E,0x3E,0x30,0x30,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -:- bbf
	0x00,0x00,0x00,0x00,0x1E,0x1E,0x1E,0x1E,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x3C,0x3C,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,  // -;-
	0x00,0x00,0x00,0x00,0x81,0xC3,0xC3,0x81,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x11,0x1B,0x0F,0x07,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00,  // -<-
	0x10,0x38,0x6C,0xC6,0x83,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x03,0x06,0x0C,0x18,0x30,0x20,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -=-
	0x00,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x08,0x18,0x30,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  // ->-
	0x00,0x00,0x00,0x00,0x00,0x01,0x83,0xC6,0x6C,0x38,0x10,0x00,
	0x20,0x30,0x18,0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00,

	0x00,0x60,0xF0,0x78,0x0C,0x04,0x04,0x04,0x0C,0xF8,0xF0,0x00,  // -?-
	0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x1C,0x07,0x03,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x30,0x79,0x30,0x00,0x00,0x00,0x00,

	0xE0,0x10,0x08,0x84,0xC4,0x44,0x44,0xC4,0x88,0x10,0xE0,0x00,  // -@-
	0xFF,0x00,0xFF,0xFF,0x80,0x80,0x80,0xFF,0xFF,0x00,0xFF,0x00,
	0x07,0x08,0x10,0x21,0x21,0x21,0x20,0x20,0x21,0x11,0x08,0x00,
  
    0x00,0x00,0x00,0xE0,0xFC,0xFC,0xFC,0xFC,0xE0,0x00,0x00,0x00,  // -A- bbf
	0x00,0x00,0xF0,0xFF,0xFF,0x80,0x80,0xFF,0xFF,0xF0,0x00,0x00,
	0x00,0x3C,0x3F,0x3F,0x3F,0x03,0x03,0x3F,0x3F,0x3F,0x3C,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x0C,0x1C,0xFC,0xFC,0xF8,0xF0,0x00,  // -B-
	0x00,0xFF,0xFF,0xFF,0xFF,0x0E,0x1F,0xFF,0xFB,0xF1,0xE0,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x1C,0x3C,0xFC,0xFC,0xF8,0xF0,0x00,  // -C- bbf 
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xE7,0xE7,0xE7,0xE7,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x1C,0x3C,0xFC,0xFC,0xF8,0xE0,0x00,  // -D- bbf
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x07,0x00,

    0x00,0xFC,0xFC,0xFC,0xFC,0x3C,0x3C,0x3C,0x3C,0x3C,0x00,0x00,  // -E-
	0x00,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,0x00,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x00,  // -F-
	0x00,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,0x00,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0xE0,0xF8,0xFC,0xFC,0x1C,0x1C,0x7C,0x7C,0x78,0x70,0x00,  // -G-
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x18,0xF8,0xF8,0xF8,0xF8,0x00,
	0x00,0x07,0x1F,0x3F,0x3F,0x38,0x3C,0x1F,0x3F,0x3F,0x3F,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,  // -H-
	0x00,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,  // -I-
	0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,  // -J-
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,
	0x00,0x00,0x00,0x38,0x38,0x3F,0x3F,0x1F,0x0F,0x00,0x00,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x80,0xE0,0xF8,0xFC,0x7C,0x1C,0x00,  // -K-
	0x00,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xF7,0xE1,0x80,0x00,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x01,0x07,0x3F,0x3F,0x3E,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -L-
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x00,

	0x00,0xFC,0xFC,0xFC,0xC0,0x00,0x00,0xC0,0xFC,0xFC,0xFC,0x00,  // -M-
	0x00,0xFF,0xFF,0xFF,0x7F,0xFC,0xFC,0x7F,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x00,0x3F,0x3F,0x00,0x3F,0x3F,0x3F,0x00,

	0x00,0xFC,0xFC,0xFC,0xF0,0x80,0x00,0xFC,0xFC,0xFC,0xFC,0x00,  // -N-
	0x00,0xFF,0xFF,0xFF,0xFF,0x1F,0xFC,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x0F,0x3F,0x3F,0x3F,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x1C,0x1C,0xFC,0xFC,0xF8,0xF0,0x00,  // -O-
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x38,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x1C,0x3C,0xFC,0xFC,0xF8,0xF0,0x00,  // -P- bbf
	0x00,0xFF,0xFF,0xFF,0xFF,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x1C,0x1C,0xFC,0xFC,0xF8,0xF0,0x00,  // -Q-
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x03,0x07,0x0F,0x0F,0x0E,0x3E,0x7F,0x7F,0x67,0x63,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x0C,0x1C,0xFC,0xFC,0xF8,0xF0,0x00,  // -R-
	0x00,0xFF,0xFF,0xFF,0xFF,0x3E,0x7F,0xFF,0xF7,0xE3,0xC1,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0xF0,0xF8,0xFC,0xFC,0x1C,0x3C,0xFC,0xFC,0xF8,0xF0,0x00,  // -S-
	0x00,0x07,0x0F,0x1F,0x3F,0x7E,0xFC,0xF8,0xF0,0xE0,0xC0,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x3C,0x3C,0x3C,0xFC,0xFC,0xFC,0xFC,0x3C,0x3C,0x3C,0x00,  // -T-
	0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,  // -U-
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x07,0x1F,0x3F,0x3F,0x38,0x38,0x3F,0x3F,0x1F,0x07,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,  // -V-
	0x00,0x7F,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0x7F,0x00,
	0x00,0x00,0x03,0x0F,0x1F,0x3E,0x3E,0x1F,0x0F,0x03,0x00,0x00,

	0x00,0xFC,0xFC,0xFC,0x00,0xFC,0xFC,0x00,0xFC,0xFC,0xFC,0x00,  // -W-
	0x00,0x3F,0xFF,0xFF,0xFC,0x7F,0x7F,0xFC,0xFF,0xFF,0x3F,0x00,
	0x00,0x00,0x03,0x3F,0x3F,0x00,0x00,0x3F,0x3F,0x03,0x00,0x00,

	0x00,0x1C,0x7C,0xFC,0xFC,0x00,0x00,0xFC,0xFC,0x7C,0x1C,0x00,  // -X-
	0x00,0x00,0x80,0xE3,0xFF,0x3F,0x7E,0xFF,0xE1,0x80,0x00,0x00,
	0x00,0x3E,0x3F,0x3F,0x3F,0x00,0x00,0x3F,0x3F,0x3F,0x3E,0x00,

	0x00,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x00,  // -Y-
	0x00,0x07,0x3F,0xFF,0xFF,0xF8,0xF8,0xFF,0xFF,0x3F,0x07,0x00,
	0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,

	0x00,0x1C,0x1C,0x1C,0x1C,0x1C,0x9C,0xFC,0xFC,0xFC,0xFC,0x00,  // -Z- bbf
	0x00,0x00,0x80,0xE0,0xF8,0xFE,0x7F,0x1F,0x07,0x03,0x00,0x00,
	0x00,0x3E,0x3F,0x3F,0x3F,0x3D,0x3C,0x3C,0x3C,0x3C,0x3C,0x00,

	0x00,0x00,0x00,0x00,0x00,0xFF,0x01,0x01,0x01,0x01,0x01,0x00,  // -[-
	0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x80,0x80,0x80,0x80,0x00,

	0x06,0x1C,0x70,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -\-
	0x00,0x00,0x00,0x01,0x07,0x1C,0x70,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1C,0x30,0x00,

	0x00,0x01,0x01,0x01,0x01,0x01,0xFF,0x00,0x00,0x00,0x00,0x00,  // -]-
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x80,0x80,0x80,0x80,0x80,0xFF,0x00,0x00,0x00,0x00,0x00,

	0x00,0x60,0x30,0x18,0x0C,0x06,0x06,0x0C,0x18,0x30,0x60,0x00,  // -^-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -_-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,

	0x00,0x00,0x02,0x03,0x07,0x06,0x04,0x0C,0x08,0x08,0x00,0x00,  // -`-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -a-
	0x00,0x1F,0x9F,0x9F,0xDF,0xE1,0x71,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x1F,0x3F,0x3F,0x3F,0x30,0x18,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -b-
	0x00,0xFF,0xFF,0xFF,0xFF,0x03,0x07,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x18,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -c-
	0x00,0xFF,0xFF,0xFF,0xFF,0x07,0x03,0x8F,0x8F,0x8F,0x8F,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x3C,0x38,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0xFC,0xFC,0xFC,0xFC,0x00,  // -d-
	0x00,0xFF,0xFF,0xFF,0xFF,0x07,0x03,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x3C,0x18,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -e-
	0x00,0xFF,0xFF,0xFF,0xFF,0x31,0x33,0x3F,0x3F,0x3F,0x3F,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x3C,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x80,0x80,0xF0,0xF8,0xFC,0x9C,0x9C,0x9C,0x9C,0x00,0x00,  // -f-
	0x00,0x03,0x03,0xFF,0xFF,0xFF,0xFF,0x03,0x03,0x03,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0x00,  // -g-
	0x00,0xFF,0xFF,0xFF,0xFF,0x03,0x87,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x71,0xF3,0xF3,0xF3,0xE3,0xE3,0xFF,0xFF,0x7F,0x3F,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,  // -h-
	0x00,0xFF,0xFF,0xFF,0xFF,0x03,0x07,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,0x00,  // -i-
	0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,  // -j-
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,
	0x00,0x00,0x00,0xE0,0xE0,0xFF,0xFF,0x7F,0x3F,0x00,0x00,0x00,

	0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x80,0xC0,0xC0,0xC0,0x00,  // -k-
	0x00,0xFF,0xFF,0xFF,0xFF,0xF8,0xFE,0xFF,0xDF,0x87,0x01,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x03,0x3F,0x3F,0x3F,0x38,0x00,

	0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,  // -l-
	0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x1F,0x3F,0x3F,0x3F,0x38,0x30,0x00,0x00,0x00,

	0x00,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,  // -m-
	0x00,0xFF,0xFF,0xFF,0x01,0xFF,0xFF,0xFF,0x01,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x00,0x3F,0x3F,0x3F,0x00,0x3F,0x3F,0x00,

	0x00,0xC0,0xC0,0xC0,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,  // -n-
	0x00,0xFF,0xFF,0xFF,0xFF,0x03,0x07,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -o-
	0x00,0xFF,0xFF,0xFF,0xFF,0x01,0x01,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x38,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -p-
	0x00,0xFF,0xFF,0xFF,0xFF,0x01,0x03,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0xFF,0xFF,0xFF,0xFF,0x1C,0x38,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0x00,  // -q-
	0x00,0xFF,0xFF,0xFF,0xFF,0x07,0x03,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x3C,0x18,0xFF,0xFF,0xFF,0xFF,0x00,

	0x00,0xC0,0xC0,0xC0,0xC0,0x00,0x80,0xC0,0xC0,0xC0,0x00,0x00,  // -r-
	0x00,0xFF,0xFF,0xFF,0xFF,0x0F,0x07,0x07,0x03,0x03,0x00,0x00,
	0x00,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,  // -s-
	0x00,0x1F,0x3F,0x7F,0x7F,0xF9,0xF0,0xE7,0xC7,0xC7,0x87,0x00,
	0x00,0x0E,0x1E,0x3E,0x3E,0x30,0x31,0x3F,0x3F,0x1F,0x0F,0x00,

	0x00,0x80,0x80,0xF8,0xF8,0xF8,0xF8,0x80,0x80,0x80,0x00,0x00,  // -t-
	0x00,0x03,0x03,0xFF,0xFF,0xFF,0xFF,0x03,0x03,0x03,0x00,0x00,
	0x00,0x00,0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x38,0x38,0x00,0x00,

	0x00,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0x00,  // -u-
	0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x0F,0x1F,0x3F,0x3F,0x38,0x1C,0x3F,0x3F,0x3F,0x3F,0x00,

	0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x00,  // -v-
	0x00,0x1F,0x7F,0xFF,0xF0,0x00,0x00,0xF0,0xFF,0x7F,0x1F,0x00,
	0x00,0x00,0x00,0x03,0x0F,0x3E,0x3E,0x0F,0x03,0x00,0x00,0x00,

	0x00,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0x00,  // -w-
	0x00,0x3F,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x3F,0x00,
	0x00,0x00,0x03,0x3F,0x3F,0x03,0x03,0x3F,0x3F,0x03,0x00,0x00,

	0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x00,  // -x-
	0x00,0x0F,0x1F,0x3F,0xF0,0xE0,0xE0,0xF0,0x7F,0x3F,0x0F,0x00,
	0x00,0x3C,0x3F,0x3F,0x07,0x00,0x00,0x07,0x3F,0x3E,0x38,0x00,

	0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x00,  // -y-
	0x00,0x1F,0xFF,0xFF,0xF0,0x00,0x00,0xF0,0xFF,0xFF,0x1F,0x00,
	0x60,0xE0,0xE0,0xE7,0xFF,0xFE,0x7E,0x0F,0x07,0x00,0x00,0x00,

	0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,  // -z- bbf
	0x00,0x01,0x01,0x01,0x81,0xE1,0xF9,0xFF,0x7F,0x1F,0x07,0x00,
	0x00,0x38,0x3C,0x3F,0x3F,0x3F,0x3B,0x38,0x38,0x38,0x38,0x00,

	0x00,0x00,0x00,0x00,0x00,0xFC,0xFE,0x03,0x01,0x01,0x01,0x00,  // -{-
	0x00,0x00,0x00,0x18,0x3C,0xE7,0xC3,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x3F,0x7F,0xC0,0x80,0x80,0x80,0x00,

	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,  // -|-
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,

	0x00,0x01,0x01,0x01,0x03,0xFE,0xFC,0x00,0x00,0x00,0x00,0x00,  // -}-
	0x00,0x00,0x00,0x00,0x00,0xC3,0xE7,0x3C,0x18,0x00,0x00,0x00,
	0x00,0x80,0x80,0x80,0xC0,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -~-
	0x10,0x18,0x0C,0x04,0x0C,0x08,0x10,0x30,0x20,0x30,0x18,0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


	0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,  // -heart-
	0x0E,0x1F,0x37,0x6F,0xDF,0xFC,0xFF,0x7F,0x3F,0x1F,0x0E,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#endif
