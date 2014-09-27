/**************************************************************************
This is a library for the S6D02A1/ST7735 display,
model TFTM1802SPI, 1.8" TFT Breakout with w/SD card.
----> http://www.creativityslashdesign.tk
 
Written by Cosimo Orlando for C/D.tk .
MIT License, Copyright (c) 2014, all text above must be included
in any redistribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
**************************************************************************/

#include <project.h>
#include "ControlLCD.h"
#include "glcdfont.c"
#include <stdlib.h>
#include <math.h>

extern uint8_t x_start, y_start;
uint8_t _width  = 0;
uint8_t _height = 0;
char character = ' ';

const SPIM_SPI_INIT_STRUCT ConfigSPIM =
{
    SPIM_SPI_MASTER,
    SPIM_SPI_MODE_MOTOROLA,
    SPIM_SPI_SCLK_CPHA0_CPOL0,
    2,
    0,
    0,
    0,
    8,
    8,
    SPIM_BITS_ORDER_MSB_FIRST,
    SPIM_SPI_TRANSFER_CONTINUOUS,
    8,
    NULL,
    8,
    NULL,
    1,
    SPIM_INTR_RX_TRIGGER,
    0,
    SPIM_INTR_TX_TRIGGER,
    0,
};

/*******************************************************/

/* This function transmits a command to the LCD driver */
void LCDCommand(uint8_t datacmd)
{
    SPIM_ClearMasterInterruptSource(SPIM_INTR_MASTER_SPI_DONE);
    RS_Write(LOW);
    SPIM_SpiUartWriteTxData(datacmd);
    while(0u == (SPIM_GetMasterInterruptSource() & SPIM_INTR_MASTER_SPI_DONE));
}
/*******************************************************/

/* This function transmits a data to the LCD driver */
void LCDData(uint8_t datadt)
{
    SPIM_ClearMasterInterruptSource(SPIM_INTR_MASTER_SPI_DONE);
    RS_Write(HIGH);
    SPIM_SpiUartWriteTxData(datadt);
    while(0u == (SPIM_GetMasterInterruptSource() & SPIM_INTR_MASTER_SPI_DONE));
}
/*******************************************************/
void ResetDispaly(void)
{
    RST_Write(HIGH);
    CyDelay(10);
    RST_Write(LOW);
    CyDelay(10);
    RST_Write(HIGH);
    CyDelay(10);
}
/*******************************************************/
void initSPIM(void)
{
    ClockSPI_Start();
    SPIM_SpiInit(&ConfigSPIM);
    SPIM_Start();
    CyDelay(100);
};
/*******************************************************/
void initDISPLAY(void)
{
    x_start = y_start = 0u;                         // May be overridden in init function.  
    initSPIM();                                     // Initialization of SPIM.
    ResetDispaly();
        
    LCDCommand(0xF0);
    LCDData(0x5A); LCDData(0x5A);                   // Excommand2.
	
    LCDCommand(0xFC);
    LCDData(0x5A); LCDData(0x5A);                   // Excommand3.
	
    LCDCommand(0x26);
    LCDData(0x01);                                  // Gamma set.
	
    LCDCommand(0xFA);
    LCDData(0x02); LCDData(0x1F); LCDData(0x00);
    LCDData(0x10); LCDData(0x22); LCDData(0x30);
    LCDData(0x38); LCDData(0x3A); LCDData(0x3A);
    LCDData(0x3A); LCDData(0x3A); LCDData(0x3A);
    LCDData(0x3D); LCDData(0x02); LCDData(0x01);	// Positive gamma control.
	
    LCDCommand(0xFB);
    LCDData(0x21); LCDData(0x00); LCDData(0x02);
    LCDData(0x04); LCDData(0x07); LCDData(0x0A);
    LCDData(0x0B); LCDData(0x0C); LCDData(0x0C);
    LCDData(0x16); LCDData(0x1E); LCDData(0x30);
    LCDData(0x3F); LCDData(0x01); LCDData(0x02);	// Negative gamma control.
	
    //////////////power setting sequence//////////
    
    LCDCommand(0xFD);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x17); LCDData(0x10); LCDData(0x00);
    LCDData(0x01); LCDData(0x01); LCDData(0x00);
    LCDData(0x1F); LCDData(0x1F);                   // Analog parameter control.
	
    LCDCommand(0xF4);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x00); LCDData(0x00); LCDData(0x3F);
    LCDData(0x3F); LCDData(0x07); LCDData(0x00);
    LCDData(0x3C); LCDData(0x36); LCDData(0x00);
    LCDData(0x3C); LCDData(0x36); LCDData(0x00);	// Power control.
	
    LCDCommand(0xF5);
    LCDData(0x00); LCDData(0x70); LCDData(0x66);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x00); LCDData(0x6D); LCDData(0x66);
    LCDData(0x06);                                  // VCOM control.
	
    LCDCommand(0xF6);
    LCDData(0x02); LCDData(0x00); LCDData(0x3F);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x02); LCDData(0x00); LCDData(0x06);
    LCDData(0x01); LCDData(0x00);                   // Source control.
	
    /////////////initializing sequence/////////////
    
    LCDCommand(0xF2);
    LCDData(0x00); LCDData(0x01); /*04*/ LCDData(0x03);
    LCDData(0x08); LCDData(0x08); LCDData(0x04);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x00); LCDData(0x00); LCDData(0x01);
    LCDData(0x00); LCDData(0x00); LCDData(0x04);
    LCDData(0x08); LCDData(0x08);                   // Display control.
	
    LCDCommand(0xF8);
    LCDData(0x11);                                  // Gate control.
	
    LCDCommand(0xF7);
    LCDData(0xC8); LCDData(0x20); LCDData(0x00);
    LCDData(0x00);	                                // Interface control.
    
    LCDCommand(0xF3);
    LCDData(0x00);
    LCDData(0x00);	                                // Power sequence control.
	
    LCDCommand(SLEEPOUT);
    CyDelay(50);                                    // Wake.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0x01);
    CyDelay(50);	                                // Power sequence control.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0x03);
    CyDelay(50);	                                // Power sequence control.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0x07);
    CyDelay(50);	                                // Power sequence control.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0x0F);
    CyDelay(50);	                                // Power sequence control.
	
    LCDCommand(0xF4);
    LCDData(0x00); LCDData(0x04); LCDData(0x00);
    LCDData(0x00); LCDData(0x00); LCDData(0x3F);
    LCDData(0x3F); LCDData(0x07); LCDData(0x00);
    LCDData(0x3C); LCDData(0x36); LCDData(0x00);
    LCDData(0x3C); LCDData(0x36); LCDData(0x00);
    CyDelay(50);	                                // Power control.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0x1F);
    CyDelay(50);	                                // Power sequence control.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0x7F);
    CyDelay(50);	                                // Power sequence control.
	
    LCDCommand(0xF3);
    LCDData(0x00); LCDData(0xFF);
    CyDelay(50);	                                // Power sequence control.
    
    LCDCommand(0xFD);
    LCDData(0x00); LCDData(0x00); LCDData(0x00);
    LCDData(0x17); LCDData(0x10); LCDData(0x00);
    LCDData(0x00); LCDData(0x01); LCDData(0x00);
    LCDData(0x16); LCDData(0x16);                   // Analog parameter control.

    LCDCommand(0xF4);
    LCDData(0x00); LCDData(0x09); LCDData(0x00);
    LCDData(0x00); LCDData(0x00); LCDData(0x3F);
    LCDData(0x3F); LCDData(0x07); LCDData(0x00);
    LCDData(0x3C); LCDData(0x36); LCDData(0x00);
    LCDData(0x3C); LCDData(0x36); LCDData(0x00);    // Power control.

    LCDCommand(MADCTL);                             // Memory access data control.
    LCDData(0x08);    
    LCDCommand(0x35);                               // Tearing effect line on.
    LCDData(NOP);
	LCDCommand(DISPON);                             // Display on.
    LCDCommand(RAMWR);                              // Memory write.
    
    _width  = WIDTH;
    _height = HEIGHT;
}
/*******************************************************/
void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  LCDCommand(CASET);      // column addr set
  LCDData(NOP);  
  LCDData(x0+x_start);    // XSTART
  LCDData(NOP);
  LCDData(x1+x_start);    // XEND
  LCDCommand(RASET);      // row addr set
  LCDData(NOP);
  LCDData(y0+y_start);    // YSTART
  LCDData(NOP);
  LCDData(y1+y_start);    // YEND

  LCDCommand(RAMWR);      // write to RAM
}
/*******************************************************/
void fillColor(uint32_t color)
{  
  SPIM_ClearMasterInterruptSource(SPIM_INTR_MASTER_SPI_DONE); // 6-6-6 color palette
  RS_Write(HIGH);
  SPIM_SpiUartWriteTxData(color);
  SPIM_SpiUartWriteTxData(color >> 8);
  SPIM_SpiUartWriteTxData(color >> 16);
  while(0u == (SPIM_GetMasterInterruptSource() & SPIM_INTR_MASTER_SPI_DONE));
}
/*******************************************************/
void drawPixel(uint16_t x, uint16_t y, uint32_t color)
{
  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
 
  setAddrWindow(x,y,x+1,y+1);
  
  fillColor(color);
}
/*******************************************************/
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  
  if (steep) { swap(x0, y0); swap(x1, y1); }

  if (x0 > x1) { swap(x0, x1); swap(y0, y1); }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++)
  {
    if (steep)
    {
      drawPixel(y0, x0, color);
    } else
    {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}
/*******************************************************/
void setWidth(uint8_t d)
{
    if (d == NULL)
        _width = WIDTH;
    else
        _width = d;
}
/*******************************************************/
void setHeight(uint8_t d)
{
    if (d == NULL)
        _height = HEIGHT;
    else
        _height = d;
}
/*******************************************************/
uint8_t getWidth(void)
{
    return _width;
}
/*******************************************************/
uint8_t getHeight(void)
{
    return _height;
}
/*******************************************************/

/* some definition... */
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

/* Set Display rotation. */
void RotSetting(uint8_t m)
{
  LCDCommand(MADCTL);
  uint8_t rotation = m % 4; // can't be higher than 3
  switch (rotation)
  { 
    case 0: // portrait 0°
    LCDData(MADCTL_BGR);
    _width  = WIDTH;
    _height = HEIGHT;
    break;
    
    case 1: // landscape 90°
    LCDData(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
    _width  = HEIGHT;
    _height = WIDTH;
    break;
    
    case 2: // portrait 180°
    LCDData(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
    _width  = WIDTH;
    _height = HEIGHT;
    break;
    
    case 3: // landscape 270°
    LCDData(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}
/*******************************************************/
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint32_t color)
{  
  // Rudimentary clipping
  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
  if((y+h-1) >= _height)  h = _height-y;
  
  setAddrWindow(x, y, x, y+h-1);

  while (h--) {
    fillColor(color);
  }
}
/*******************************************************/
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint32_t color)
{
  // Rudimentary clipping
  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--) {
    fillColor(color);
  }
}
/*******************************************************/
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}
/*******************************************************/
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
  int16_t i;
  for (i = x; i < x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}
/*******************************************************/
void fillScreen(uint32_t color)
{
  fillRect(0, 0, _width, _height, color);
}
/*******************************************************/
void lcdTest(void)
{
  uint8_t i;
  for (i = 0; i < 100; i++) drawPixel(i, i, WHITE);
}
/*******************************************************/
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}
/*******************************************************/
void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint32_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}
/*******************************************************/

/* Used to do circles and roundrects. */
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}
/*******************************************************/
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color)
{
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}
/*******************************************************/

/* Fill a rounded rectangle. */
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint32_t color)
{
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}
/*******************************************************/
char* toChar(uint8_t d)
{
    character = (char)(d+48);
    if(d < 10) return &character;
    return 0;
}
/*******************************************************/
void drawChar(int16_t x, int16_t y, unsigned char c, uint32_t color, uint16_t bg, uint8_t size)
{
  if((x >= _width)            || // Clip right
    (y  >= _height)           || // Clip bottom
    ((x + 6 * size - 1) < 0)  || // Clip left
    ((y + 8 * size - 1) < 0))    // Clip top
    return;

  int8_t i;

  for (i = 0; i < 6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = font[c*5+i];
    int8_t j;
    for (j = 0; j < 8; j++) {
      if (line & 0x1) {
        if (size == 1)          // default size
          drawPixel(x+i, y+j, color);
        else {                  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1)          // default size
          drawPixel(x+i, y+j, bg);
        else {                  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}
/*******************************************************/
void print(char* text, uint32_t color, uint16_t bg, uint8_t size)
{
    uint8_t i, j, nChar;
    i = j = 0;
    nChar = ' ';
    
    for(j = 0; j < strlen(text); j++)
    {
        if (text[j] == '\n')
        {
            j++;
            i = x_start = 0;    // x reset
            y_start += size*8;  // new line
        }
        else if (text[j] == '\r')
        {
            // skip em
        }
        
        if (_height == HEIGHT)
            nChar = 21;
        else if (_height == WIDTH)
            nChar = 26;
        
        if (i == nChar)
        {
            i = x_start = 0;
            y_start += size*8; // new line
        }
        
        x_start = i+5*i*size;
        drawChar(0, 0, text[j], color, bg, size);    
        i++;
    }
    x_start = 0;
    y_start = 0;
}
/*******************************************************/
void printN(char* textTwo, uint32_t color, uint16_t bg, uint8_t size, uint8_t num)
{
    char tempNum[1];
    char tempText[strlen(textTwo)];
    
    uint8_t c = num/100;
    uint8_t d = num/10 - c*10;
    uint8_t u = num - c*100 - d*10;

    if ((num >= 100)&&(num <= 255))
    {
        strncat(tempNum, toChar(c), 1);
        strncat(tempNum, toChar(d), 1);
        strncat(tempNum, toChar(u), 1);
    }
    else if ((num >= 10)&&(num < 100))
    {
        strncat(tempNum, toChar(d), 1);
        strncat(tempNum, toChar(u), 1);
    }
    else
    strncat(tempNum, toChar(u), 1);

    strcpy(tempText, textTwo);
    
    strncat(tempText, tempNum, strlen(tempNum));
    print(tempText, color, bg, size);
}