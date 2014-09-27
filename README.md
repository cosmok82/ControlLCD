ControlLCD
==========

TFTM1802 SPI Display Library for CY8CKIT-049-42xx

<div style="text-align: center;">
<a href="http://3.bp.blogspot.com/-NKO-7JuCFo4/U_nTT84kACI/AAAAAAAAEHA/nCHZ5gU6J5Y/s1600/cover.jpg">
<img alt="img" src="http://3.bp.blogspot.com/-NKO-7JuCFo4/U_nTT84kACI/AAAAAAAAEHA/nCHZ5gU6J5Y/s1600/cover.jpg">
</a>
</div>
<br>
Author: Orlando Cosimo.<br>
Blog: http://creativityslashdesign.tk/
<br>
<br>
Description:
Provides a demo of SPI LCD Library functionality on a display TFTM1802 (S6D02A1\ST7735), tested with the board CY8CKIT-049-42xx.
<br>
CHANGELOG (notable new features or fixes)
---------

### Beta 1.1
Added:
* Add Adafruit font library (Thanks to Adafruit GFX :D )
* void setWidth(uint8_t d)
* void setHeight(uint8_t d)
* uint8_t getWidth(void)
* uint8_t getHeight(void)
* void RotSetting(uint8_t m)
	(!) it sets the display rotation from 0° to 270° degrees
* char* toChar(uint8_t d)
	(!) it converts a number from 0 to 9 in a char
* void drawChar(int16_t x, int16_t y, unsigned char c, uint32_t color, uint16_t bg, uint8_t size)
	(!) it draws a char to display
* void print(char* text, uint32_t color, uint16_t bg, uint8_t size)
	(!) it prints a formatted text in according to screen size
* void printN(char* text, uint32_t color, uint16_t bg, uint8_t size, uint8_t num)
	(!) it appends to text a number and prints out everything
<br>
All info on my blog.
<br>
<br>
<br>
MIT License, Copyright (c) 2014.
