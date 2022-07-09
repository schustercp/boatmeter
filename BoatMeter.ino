
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include <SPI.h>

#include "Fonts/FreeSansBoldOblique_60pt7b.h"
#include "Fonts/FreeSansBoldOblique_32pt7b.h"

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_DC         3

// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:

//Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst);
Adafruit_ST7789 tft1 = Adafruit_ST7789(7, TFT_DC, 6);
Adafruit_ST7789 tft2 = Adafruit_ST7789(2, TFT_DC, 4);

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

  // initialize the serial communication:
  SerialUSB.begin(9600);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
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

  /* Enable the APB clock for the ADC. */
  PM->APBCMASK.reg |= PM_APBCMASK_ADC;

  ADC->REFCTRL.reg = 0x80;                         // Enable Ref Comp and use (INT1V) 1.0V voltage reference
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV512 |    // Divide Clock ADC GCLK by 512 (48MHz/512 = 93.7kHz)
                   ADC_CTRLB_RESSEL_16BIT;         // Set ADC resolution to 16 bits
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
  ADC->SAMPCTRL.reg = 0x00;                        // Set max Sampling Time Length to half divided ADC clock pulse (5.33us)
  ADC->INPUTCTRL.reg = 0x00000000;                 // Set the analog input to A0 and to Scan 1 Channel
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_256 |   // Accumulate 256 samples for 16-bit resolution
                     ADC_AVGCTRL_ADJRES(0);        // Zero result adjust
  ADC->CTRLA.bit.ENABLE = 0x01;                    // Enable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  if (SerialUSB.available() > 0)
  {
    SerialUSB.print("Hello");
  }
}

void loop() 
{
  while(SerialUSB.available() == 0) 
  {
  }

  SerialUSB.print("Hello");
    
  float voltage1 = 0.0;
  float voltage2 = 0.0;
  float current = 0.0; //mAmps
  //float power   = 0.0; //mWatts

  ADC->INPUTCTRL.reg = 0x00000000;                 // Set the analog input to A0
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization
  while(!ADC->INTFLAG.bit.RESRDY);                 // Wait for the conversion to complete
  uint16_t A0_result = ADC->RESULT.reg;            // Read the ADC result,  This Clears the result ready (RESRDY) interrupt flag

  ADC->INPUTCTRL.reg = 0x00000001;                 // Set the analog input to A1
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization
  while(!ADC->INTFLAG.bit.RESRDY);                 // Wait for the conversion to complete
  uint16_t A1_result = ADC->RESULT.reg;            // Read the ADC result,  This Clears the result ready (RESRDY) interrupt flag

  if (SerialUSB.available() > 0)
  {
    SerialUSB.print(A0_result);
  }

  voltage1 = (A0_result / 65536.0) / 1192.0 * 23122;
  voltage2 = (A1_result / 65536.0) / 1192.0 * 23122;

  if (voltage1 < 0.0)
  {
    voltage1 = 0.0;
  }
  else if (voltage1 > 20.0)
  {
    voltage1 = 99.9;
  }

  if (voltage2 < 0.0)
  {
    voltage2 = 0.0;
  }
  else if (voltage2 > 20.0)
  {
    voltage2 = 99.9;
  }

  current = (voltage1 - voltage2) / 0.100;
  //power = voltage1 * current; //Convert to Watts

  if (current < 0.0)
  {
    current = 0.0;
  }
  else if (current > 99.9)
  {
    current = 99.9;
  }

  uint8_t tens = (uint8_t)(voltage1 / 10.0);
  voltage1 -= (float)tens * 10.0;
  uint8_t ones = (uint8_t)voltage1;
  voltage1 -= (float)ones;
  voltage1 *= 10.0;
  voltage1 += 0.5; //Round Up
  uint8_t tenths = (uint8_t)voltage1;

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
