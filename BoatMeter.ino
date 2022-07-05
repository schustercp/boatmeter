
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include <SPI.h>

#include "Fonts/FreeSansBoldOblique_60pt7b.h"
#include "Fonts/FreeSansBoldOblique_32pt7b.h"

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_DC         3

// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:
Adafruit_ST7789 tft1 = Adafruit_ST7789(7, TFT_DC, 6);
Adafruit_ST7789 tft2 = Adafruit_ST7789(2, TFT_DC, 1);

float p = 3.1415926;

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

unsigned char v_lastCharPos0 = '0';
unsigned char v_lastCharPos1 = '0';
unsigned char v_lastCharPos2 = '0';

unsigned char a_lastCharPos0 = '0';
unsigned char a_lastCharPos1 = '0';
unsigned char a_lastCharPos2 = '0';

void setup(void)
{
  // Use this initializer if using a 1.69" 280x240 TFT:
  tft1.init(240, 280);           // Init ST7789 280x240
  tft2.init(240, 280);           // Init ST7789 280x240

  tft1.setRotation(1);
  tft1.fillScreen(ST77XX_BLACK);
  tft1.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);

  tft2.setRotation(1);
  tft2.fillScreen(ST77XX_BLACK);
  tft2.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);
 
  int16_t x = 3;
  int16_t y = 200;
  tft1.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft1.drawChar(x,       y, 'D', ST77XX_RED, ST77XX_BLACK,  1, 1);
  tft1.drawChar(x + 45,  y, 'C', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 105, y, 'V', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 145, y, 'o', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 185, y, 'l', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 205, y, 't', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 228, y, 's', ST77XX_RED, ST77XX_BLACK, 1, 1);

  tft2.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft2.drawChar(x,       y, 'D', ST77XX_RED, ST77XX_BLACK,  1, 1);
  tft2.drawChar(x + 45,  y, 'C', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 105, y, 'A', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 148, y, 'm', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 200, y, 'p', ST77XX_RED, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 235, y, 's', ST77XX_RED, ST77XX_BLACK, 1, 1);

  y = 115;
  tft1.setFont(&FreeSansBoldOblique_BYJ360pt7b);
  tft2.setFont(&FreeSansBoldOblique_BYJ360pt7b);
  
  tft1.drawChar(18,  y, v_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(86,  y, v_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(148, y, '.', ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(178, y, v_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK, 1, 1);

  tft2.drawChar(18,  y, a_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(86,  y, a_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(148, y, '.', ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(178, y, a_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
}

void loop() 
{
  float voltage = 0.0; //mVolts
  float current = 0.0; //mAmps
  float power   = 0.0; //mWatts

  voltage /= 1000.0; //Convert to Volts
  current /= 1000.0; //Convert to Amps
  power   /= 1000.0; //Convert to Watts

  uint8_t tens = (uint8_t)(voltage / 10.0);
  voltage -= (float)tens * 10.0;
  uint8_t ones = (uint8_t)voltage;
  voltage -= (float)ones;
  voltage *= 10.0;
  voltage += 0.5; //Round Up
  uint8_t tenths = (uint8_t)voltage;

  unsigned char digit = 0x30 + tens;
  if(v_lastCharPos0 != digit)
  {
    tft1.drawChar2(18, 115, digit, v_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK);
    v_lastCharPos0 = digit;
  }

  digit = 0x30 + ones;
  if(v_lastCharPos1 != digit)
  {
    tft1.drawChar2(86, 115, digit, v_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK);
    v_lastCharPos1 = digit;
  }

  digit = 0x30 + tenths;
  if(v_lastCharPos2 != digit)
  {
    tft1.drawChar2(178, 115, digit, v_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK);
    v_lastCharPos2 = digit;
  }

  tens = (uint8_t)(current / 10.0);
  current -= (float)tens * 10.0;
  ones = (uint8_t)current;
  current -= (float)ones;
  current *= 10.0;
  current += 0.5; //Round Up
  tenths = (uint8_t)current;

  digit = 0x30 + tens;
  if(a_lastCharPos0 != digit)
  {
    tft2.drawChar2(18, 115, digit, a_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK);
    a_lastCharPos0 = digit;
  }

  digit = 0x30 + ones;
  if(a_lastCharPos1 != digit)
  {
    tft2.drawChar2(86, 115, digit, a_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK);
    a_lastCharPos1 = digit;
  }

  digit = 0x30 + tenths;
  if(a_lastCharPos2 != digit)
  {
    tft2.drawChar2(178, 115, digit, a_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK);
    a_lastCharPos2 = digit;
  }
  
  delay(50);
}
