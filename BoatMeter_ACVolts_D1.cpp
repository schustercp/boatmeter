#ifdef AC_Volts
#ifdef DISPLAY1

#include "BoatMeter_Common.hpp"
#include "Adafruit_GFX.h"    // Core graphics library
#include <SPI.h>
#include <Wire.h>

#include "Fonts/FreeSansBoldOblique_60pt7b.h"
#include "Fonts/FreeSansBoldOblique_32pt7b.h"
#include "Fonts/FreeSansBoldOblique_40pt7b.h"

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

void receiveEvent(int howMany);

DATA_LINE L1_Amps  = {{'3','0','.','9','~','~'},29.9, 75,75,{15,65,118,142,200,0},2,1};
DATA_LINE L2_Amps  = {{'2','9','.','9','~','~'},30.5,150,75,{15,65,118,142,200,0},2,1};

DATA_LINE L1_Watts = {{'3','0','8','8','~','~'},1234, 75,80,{15,52,90,125,160,0},4,0};
DATA_LINE L2_Watts = {{'2','8','9','9','~','~'},9876,150,80,{15,52,90,125,160,0},4,0};

void DrawAmps(void)
{
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);
 
  int16_t x = 5;
  int16_t y = 215;
  tft.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft.drawChar(x,       y, 'A', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 43,  y, 'C', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 100, y, 'A', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 143, y, 'm', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 195, y, 'p', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 230, y, 's', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft.drawChar(x,       L1_Amps.y, 'L', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 40,  L1_Amps.y, '1', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft.drawChar(x,       L2_Amps.y, 'L', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 40,  L2_Amps.y, '2', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft.setFont(&FreeSansBoldOblique_BYJ340pt7b);

  tft.drawChar(L1_Amps.x[0] + L1_Amps.offset, L1_Amps.y, L1_Amps.lastCharPos[0], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L1_Amps.x[1] + L1_Amps.offset, L1_Amps.y, L1_Amps.lastCharPos[1], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L1_Amps.x[2] + L1_Amps.offset, L1_Amps.y, L1_Amps.lastCharPos[2], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L1_Amps.x[3] + L1_Amps.offset, L1_Amps.y, L1_Amps.lastCharPos[3], ST77XX_WHITE, ST77XX_BLACK, 1, 1);

  tft.drawChar(L2_Amps.x[0] + L2_Amps.offset, L2_Amps.y, L2_Amps.lastCharPos[0], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L2_Amps.x[1] + L2_Amps.offset, L2_Amps.y, L2_Amps.lastCharPos[1], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L2_Amps.x[2] + L2_Amps.offset, L2_Amps.y, L2_Amps.lastCharPos[2], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L2_Amps.x[3] + L2_Amps.offset, L2_Amps.y, L2_Amps.lastCharPos[3], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
}

void DrawWatts(void)
{
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);
 
  int16_t x = 5;
  int16_t y = 215;
  tft.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft.drawChar(x,       y, 'A', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 43,  y, 'C', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 100, y, 'W', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 158, y, 'a', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 195, y, 't', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 220, y, 't', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft.drawChar(x,       L1_Watts.y, 'L', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 40,  L1_Watts.y, '1', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft.drawChar(x,       L2_Watts.y, 'L', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 40,  L2_Watts.y, '2', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft.drawChar(L1_Watts.x[0] + L1_Watts.offset, L1_Watts.y, L1_Watts.lastCharPos[0], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L1_Watts.x[1] + L1_Watts.offset, L1_Watts.y, L1_Watts.lastCharPos[1], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L1_Watts.x[2] + L1_Watts.offset, L1_Watts.y, L1_Watts.lastCharPos[2], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L1_Watts.x[3] + L1_Watts.offset, L1_Watts.y, L1_Watts.lastCharPos[3], ST77XX_WHITE, ST77XX_BLACK, 1, 1);

  tft.drawChar(L2_Watts.x[0] + L2_Watts.offset, L2_Watts.y, L2_Watts.lastCharPos[0], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L2_Watts.x[1] + L2_Watts.offset, L2_Watts.y, L2_Watts.lastCharPos[1], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L2_Watts.x[2] + L2_Watts.offset, L2_Watts.y, L2_Watts.lastCharPos[2], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(L2_Watts.x[3] + L2_Watts.offset, L2_Watts.y, L2_Watts.lastCharPos[3], ST77XX_WHITE, ST77XX_BLACK, 1, 1);
}

void setup(void)
{
  Wire.begin(0xC1); // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent); // register event

  // Use this initializer if using a 1.69" 280x240 TFT:
  tft.init(240, 280);           // Init ST7789 280x240
  delay(1);
  tft.setRotation(1);
  delay(1);

  DrawAmps();

  while(!Serial) { }
  Serial.begin(9600);

  Serial.println("Starting");
}

unsigned long _lastRead = 0;
const unsigned long UPDATE_TIME = 8000;
bool AmpsWatts = true;

void loop()
{ 

  // If we read before the update time limit, do not update
  if( (unsigned long)(millis() - _lastRead)  >  UPDATE_TIME)
  {
    // Record current time as _lastRead
    _lastRead = millis();
    AmpsWatts = !AmpsWatts;

    if(AmpsWatts)
    {
      DrawAmps();
    }
    else
    {
      DrawWatts();
    }
  } 

  if(AmpsWatts)
  {
    Update(L1_Amps);
    Update(L2_Amps);
  }
  else
  {
    Update(L1_Watts);
    Update(L2_Watts);
  }

  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  FloatBytes voltage;
  FloatBytes current;
  FloatBytes power;
  FloatBytes energy;
  FloatBytes frequency;
  FloatBytes powerFactor;

  Serial.print(howMany);
  Serial.println(" Bytes;");

  while(Wire.available()) // loop through all but the last
  {
    char b = Wire.read(); // receive byte as a character
    if(b == 'B')
    {
      char e = Wire.read(); // receive byte as a character
      if(e == 'E')
      {
        char g = Wire.read(); // receive byte as a character
        if(g == 'G')
        {
          break;
        }
      }
    }
  }

  if(!Wire.available())
  {
    return;
  }

  char n = Wire.read(); // receive byte as a character
  voltage.bytes[0] = Wire.read();
  voltage.bytes[1] = Wire.read();
  voltage.bytes[2] = Wire.read();
  voltage.bytes[3] = Wire.read();

  current.bytes[0] = Wire.read();
  current.bytes[1] = Wire.read();
  current.bytes[2] = Wire.read();
  current.bytes[3] = Wire.read();

  power.bytes[0] = Wire.read();
  power.bytes[1] = Wire.read();
  power.bytes[2] = Wire.read();
  power.bytes[3] = Wire.read();

  energy.bytes[0] = Wire.read();
  energy.bytes[1] = Wire.read();
  energy.bytes[2] = Wire.read();
  energy.bytes[3] = Wire.read();

  frequency.bytes[0] = Wire.read();
  frequency.bytes[1] = Wire.read();
  frequency.bytes[2] = Wire.read();
  frequency.bytes[3] = Wire.read();

  powerFactor.bytes[0] = Wire.read();
  powerFactor.bytes[1] = Wire.read();
  powerFactor.bytes[2] = Wire.read();
  powerFactor.bytes[3] = Wire.read();

  if(n == '1')
  {
    L1_Amps.Value = current;
    L1_Watts.Value = power;
  }
  else if(n == '2')
  {
    L2_Amps.Value = current;
    L2_Watts.Value = power;
  }
  //Read the LAst 4 bytes that are the End.
  Wire.read(); // receive byte as a character
  Wire.read(); // receive byte as a character
  Wire.read(); // receive byte as a character
  Wire.read(); // receive byte as a character
}

#endif
#endif