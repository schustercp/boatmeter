#ifdef AC_Volts
#ifdef DISPLAY0

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include <SPI.h>
#include "PZEM004Tv30.h"
#include <Wire.h>

#include "Fonts/FreeSansBoldOblique_60pt7b.h"
#include "Fonts/FreeSansBoldOblique_32pt7b.h"
#include "Fonts/FreeSansBoldOblique_40pt7b.h"

union FloatBytes 
{
    float f;
    unsigned char bytes[sizeof(float)];
};

// Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:
//Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst);
Adafruit_ST7789 tft = Adafruit_ST7789(0, 2, 1);

//PZEM004T* pzem;
PZEM004Tv30* pzem;
//IPAddress ip(192,168,1,1);
const uint8_t pzemAddress = 0xA5;

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

unsigned char v1_lastCharPos0 = '-';
unsigned char v1_lastCharPos1 = '-';
unsigned char v1_lastCharPos2 = '-';

unsigned char v2_lastCharPos0 = '0';
unsigned char v2_lastCharPos1 = '0';
unsigned char v2_lastCharPos2 = '0';

unsigned char v3_lastCharPos0 = '0';
unsigned char v3_lastCharPos1 = '0';
unsigned char v3_lastCharPos2 = '0';

const int topY = 65;
const int botY = 138;

const int x1 = 15;
const int x2 = 65;
const int x3 = 118;
const int x4 = 142;
const int x5 = 200;

const double errorCorrect = 1.013;

void setup(void)
{
  Wire.begin(0xC0); // join i2c bus (address optional for master)

  while(!Serial) { }
  Serial.begin(9600);

  Serial.println("Starting");

  while(!Serial1) { }
  //pzem = new PZEM004T(&Serial1);
  pzem = new PZEM004Tv30(Serial1);

  Serial.println("After PZEM Init");

  // Use this initializer if using a 1.69" 280x240 TFT:
  tft.init(240, 280);           // Init ST7789 280x240
  delay(1);

  Serial.println("After TFT Init");

  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);

  delay(1);
 
  int16_t x = 3;
  int16_t y = 200;
  tft.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft.drawChar(x,       y, 'A', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft.drawChar(x + 45,  y, 'C', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 105, y, 'V', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 145, y, 'o', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 185, y, 'l', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 205, y, 't', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft.drawChar(x + 228, y, 's', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  y = 115;
  tft.setFont(&FreeSansBoldOblique_BYJ360pt7b);
  
  tft.drawChar(18,  y, v1_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(86,  y, v1_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft.drawChar(154, y, v1_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
}

FloatBytes voltage1;
FloatBytes current1;
FloatBytes power1;
FloatBytes energy1;
FloatBytes frequency1;
FloatBytes pf1;

void loop() 
{ 
  voltage1.f   = ceil(pzem->voltage());
  current1.f   = ceil(pzem->current());
  power1.f     = ceil(pzem->power());
  energy1.f    = ceil(pzem->energy());
  frequency1.f = ceil(pzem->frequency());
  pf1.f        = ceil(pzem->pf());

  if(voltage1.f > 300)
  {
    voltage1.f = NAN;
  }

  uint8_t houndreds = 0x2D - 0x30;
  uint8_t tens = 0x2D - 0x30;
  uint8_t ones = 0x2D - 0x30;

  float fToDispaly = voltage1.f;

  if(!isnan(fToDispaly))
  {
    Serial.print(fToDispaly);
    Serial.print("V; ");

    houndreds = 0;
    tens = 0;
    ones = 0;

    //Display the house battery voltage
    houndreds = (uint8_t)(fToDispaly / 100.0);
    fToDispaly -= (float)houndreds * 100.0;
    tens = (uint8_t)(fToDispaly / 10.0);
    fToDispaly -= (float)tens * 10.0;
    ones = (uint8_t)fToDispaly;
  }

  unsigned char digit = 0x30 + houndreds;
  if(v1_lastCharPos0 != digit)
  {
    tft.drawChar2(18, 115, digit, v1_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK);
    v1_lastCharPos0 = digit;
  }

  digit = 0x30 + tens;
  if(v1_lastCharPos1 != digit)
  {
    tft.drawChar2(86, 115, digit, v1_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK);
    v1_lastCharPos1 = digit;
  }

  digit = 0x30 + ones;
  if(v1_lastCharPos2 != digit)
  {
    tft.drawChar2(154, 115, digit, v1_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK);
    v1_lastCharPos2 = digit;
  }

  Wire.beginTransmission(0xC1); // transmit to device #4
  Wire.write("BEG1");        // sends five bytes
  Wire.write(voltage1.bytes, sizeof(float));
  Wire.write(current1.bytes, sizeof(float));
  Wire.write(power1.bytes, sizeof(float));
  Wire.write(energy1.bytes, sizeof(float));
  Wire.write(frequency1.bytes, sizeof(float));
  Wire.write(pf1.bytes, sizeof(float));
  Wire.write("END1");
  Wire.endTransmission();    // stop transmitting
  
  delay(50);
}

#endif
#endif