
#include "BoatMeter_Common.hpp"
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789

#include "Fonts/FreeSansBoldOblique_60pt7b.h"
#include "Fonts/FreeSansBoldOblique_32pt7b.h"
#include "Fonts/FreeSansBoldOblique_40pt7b.h"

void Update(DATA_LINE& line)
{
  uint8_t thousands = 0x2D - 0x30;
  uint8_t hundreds = 0x2D - 0x30;
  uint8_t tens = 0x2D - 0x30;
  uint8_t ones = 0x2D - 0x30;
  uint8_t tenths = 0x2D - 0x30;
  uint8_t hundredths = 0x2D - 0x30;

  float floatToDispaly = line.Value.f;

  if(!isnan(floatToDispaly))
  {
    thousands = 0;
    hundreds = 0;
    tens = 0;
    ones = 0;
    tenths = 0;
    hundredths = 0;

    thousands = (uint8_t)(floatToDispaly / 1000.0);
    floatToDispaly -= (float)thousands * 1000.0;
    hundreds = (uint8_t)(floatToDispaly / 100.0);
    floatToDispaly -= (float)hundreds * 100.0;
    tens = (uint8_t)(floatToDispaly / 10.0);
    floatToDispaly -= (float)tens * 10.0;
    ones = (uint8_t)floatToDispaly;
    floatToDispaly -= (float)ones * 1.0;
    tenths = (uint8_t)(floatToDispaly * 10.0);
    floatToDispaly -= (float)tenths / 10.0;
    hundredths = (uint8_t)(floatToDispaly * 100.0);
  }

  int posIndex = 0;
  bool msd = false;

  if(line.beforeDec >= 4)
  {
    unsigned char digit = 0x30 + thousands;
    if(line.lastCharPos[posIndex] != digit)
    {
      if(digit == 0x30)
      { //If digit is zero display space
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, ' ', line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
      }
      else
      {
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, digit, line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
        msd = true;
      }
      line.lastCharPos[posIndex] = digit;
    }
    posIndex++;
  }

  if(line.beforeDec >= 3)
  {
    unsigned char digit = 0x30 + hundreds;
    if(line.lastCharPos[posIndex] != digit)
    {
      if(digit == 0x30 && !msd)
      { //If digit is zero display space
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, ' ', line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
      }
      else
      {
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, digit, line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
        msd = true;
      }
      line.lastCharPos[posIndex] = digit;
    }
    posIndex++;
  }

  if(line.beforeDec >= 2)
  {
    unsigned char digit = 0x30 + tens;
    if(line.lastCharPos[posIndex] != digit)
    {
      if(digit == 0x30 && !msd)
      { //If digit is zero display space
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, ' ', line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
      }
      else
      {
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, digit, line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
        msd = true;
      }
      line.lastCharPos[posIndex] = digit;
    }
    posIndex++;
  }

  if(line.beforeDec >= 1)
  {
    unsigned char digit = 0x30 + ones;
    if(line.lastCharPos[posIndex] != digit)
    {
      if(digit == 0x30 && !msd)
      { //If digit is zero display space
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, ' ', line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
      }
      else
      {
        tft.drawChar2(line.x[posIndex] + line.offset, line.y, digit, line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
        msd = true;
      }
      line.lastCharPos[posIndex] = digit;
    }
    posIndex++;
  }

  if(line.afterDec >= 1)
  {
    posIndex++; //Move past Decimal Point
    unsigned char digit = 0x30 + tenths;
    if(line.lastCharPos[posIndex] != digit)
    {
      tft.drawChar2(line.x[posIndex] + line.offset, line.y, digit, line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
      line.lastCharPos[posIndex] = digit;
    }
    posIndex++;
  }

  if(line.afterDec >= 2)
  {
    unsigned char digit = 0x30 + hundredths;
    if(line.lastCharPos[posIndex] != digit)
    {
      tft.drawChar2(line.x[posIndex] + line.offset, line.y, digit, line.lastCharPos[posIndex], ST77XX_WHITE, ST77XX_BLACK);
      line.lastCharPos[posIndex] = digit;
    }
    posIndex++;
  }
}
