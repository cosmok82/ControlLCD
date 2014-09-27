/*
*
* Author: Orlando Cosimo
* ----> http://www.creativityslashdesign.tk
*
*/

#include <cytypes.h>

//********************************************************************
//					LCD Dimension Definitions
////******************************************************************
#define WIDTH	128
#define HEIGHT	160

//********************************************************************
//					ST7735 Controller Definitions
//********************************************************************
//LCD Commands
#define	NOP         0x00u	// No operation
#define SWRESET     0x01u   // Software Reset
#define RDDID       0x04u
#define RDDST       0x09u
#define SLEEPIN     0x10u	// Sleep in
#define	SLEEPOUT	0x11u	// Sleep out
#define	NORON		0x13u	// Normal display mode on
#define	INVOFF		0x20u	// Display inversion off
#define INVON      	0x21u	// Display inversion on
#define	SETCON		0x25u	// Set contrast
#define DISPOFF     0x28u	// Display off
#define DISPON      0x29u	// Display on
#define CASET       0x2Au	// Column address set
#define RASET       0x2Bu	// Page address set
#define RAMWR       0x2Cu	// Memory write
#define RAMRD       0x2Eu   // Memory read
#define PTLAR       0x30u
#define	COLMOD		0x3Au	// Interface pixel format
#define	MADCTL		0x36u	// Memory data access control
#define FRMCTR1     0xB1u
#define FRMCTR2     0xB2u
#define FRMCTR3     0xB3u
#define INVCTR      0xB4u
#define DISSET5     0xB6u
#define PWCTR1      0xC0u   // Internal or external oscillator
#define PWCTR2      0xC1u
#define PWCTR3      0xC2u
#define PWCTR4      0xC3u
#define PWCTR5      0xC4u
#define PWCTR6      0xFCu
#define VMCTR1      0xC5u
#define RDID1       0xDAu
#define RDID2       0xDBu
#define RDID3       0xDCu
#define RDID4       0xDDu
#define GMCTRP1     0xE0u
#define GMCTRN1     0xE1u

//*******************************************************
//					16-Bit Color Definitions
//*******************************************************
#define BLACK	    0x000000u
#define NAVY        0x000080u
#define BLUE        0x0000FCu
#define TEAL        0x008080u
#define EMERALD	    0x50C878u
#define	GREEN       0x00FC00u
#define CYAN        0x008888u
#define SLATE 	    0u
#define INDIGO      0u
#define TURQUOISE   0u
#define OLIVE 		0u
#define MAROON 		0u
#define PURPLE 		0u
#define GRAY        0u
#define SKYBLUE		0u
#define BROWN		0u
#define CRIMSON 	0u
#define ORCHID 		0u
#define RED         0xFC0000u
#define MAGENTA     0xFC54A0u
#define ORANGE 		0u
#define PINK		0u
#define CORAL 		0u
#define SALMON 		0u
#define GOLD 		0u
#define YELLOW      0x00FCFCu
#define WHITE       0xFCFCFCu

//********************************************************************
//					    Various Definitions
////******************************************************************

#define LOW  0u
#define HIGH 1u

//********************************************************************
//					      Code Definitions
////******************************************************************

#define swap(a, b) { int16_t t = a; a = b; b = t; }

//*******************************************************

void LCDCommand(uint8_t datacmd);
void LCDData(uint8_t datadt);

void ResetDispaly(void);
void initSPIM(void);
void initDISPLAY(void);

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void fillColor(uint32_t color);
void drawPixel(uint16_t x, uint16_t y, uint32_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
void setWidth(uint8_t d);
void setHeight(uint8_t d);
uint8_t getWidth(void);
uint8_t getHeight(void);
void RotSetting(uint8_t m);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint32_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint32_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
void fillScreen(uint32_t color);
void lcdTest(void);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint32_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint32_t color);
char* toChar(uint8_t d);
void drawChar(int16_t x, int16_t y, unsigned char c, uint32_t color, uint16_t bg, uint8_t size);
void print(char* text, uint32_t color, uint16_t bg, uint8_t size);
void printN(char* text, uint32_t color, uint16_t bg, uint8_t size, uint8_t num);
