
#ifdef DC_Volts
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include <SPI.h>
// #include <Adafruit_NeoPixel.h>

#include "Fonts/FreeSansBoldOblique_60pt7b.h"
#include "Fonts/FreeSansBoldOblique_32pt7b.h"
#include "Fonts/FreeSansBoldOblique_40pt7b.h"

// Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_DC         3

// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:

//Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst);
Adafruit_ST7789 tft1 = Adafruit_ST7789(7, TFT_DC, 6);
Adafruit_ST7789 tft2 = Adafruit_ST7789(5, TFT_DC, 4); //This was 2

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

unsigned char v1_lastCharPos0 = '0';
unsigned char v1_lastCharPos1 = '0';
unsigned char v1_lastCharPos2 = '0';

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
  Serial.begin(115200);

  delay(1);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  delay(1);
  // Use this initializer if using a 1.69" 280x240 TFT:
  tft2.init(240, 280);           // Init ST7789 280x240
  delay(1);
  tft1.init(240, 280);           // Init ST7789 280x240
  delay(1);

  tft1.setRotation(1);
  tft1.fillScreen(ST77XX_BLACK);
  tft1.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);

  delay(1);

  tft2.setRotation(1);
  tft2.fillScreen(ST77XX_BLACK);
  tft2.drawRoundRect(0, 0, 280, 240, 42, ST77XX_CYAN);

  delay(1);
 
  int16_t x = 3;
  int16_t y = 200;
  tft1.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft1.drawChar(x,       y, 'D', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft1.drawChar(x + 45,  y, 'C', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 105, y, 'V', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 145, y, 'o', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 185, y, 'l', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 205, y, 't', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(x + 228, y, 's', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  tft2.setFont(&FreeSansBoldOblique_BYJ332pt7b);
  tft2.drawChar(x,       y, 'D', ST77XX_ORANGE, ST77XX_BLACK,  1, 1);
  tft2.drawChar(x + 45,  y, 'C', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 105, y, 'V', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 145, y, 'o', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 185, y, 'l', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 205, y, 't', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x + 228, y, 's', ST77XX_ORANGE, ST77XX_BLACK, 1, 1);

  y = 115;
  tft1.setFont(&FreeSansBoldOblique_BYJ360pt7b);
  tft2.setFont(&FreeSansBoldOblique_BYJ340pt7b);
  
  tft1.drawChar(18,  y, v1_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(86,  y, v1_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(148, y, '.', ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft1.drawChar(178, y, v1_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK, 1, 1);

  tft2.drawChar(x1, topY, v2_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x2, topY, v2_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x3, topY, '.', ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x4, topY, v2_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x5, topY, 'S', ST77XX_GREEN, ST77XX_BLACK, 1, 1);

  tft2.drawChar(x1, botY, v3_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x2, botY, v3_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x3, botY, '.', ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x4, botY, v3_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK, 1, 1);
  tft2.drawChar(x5, botY, 'P', ST77XX_RED, ST77XX_BLACK, 1, 1);

  /* Enable the APB clock for the ADC. */
  PM->APBCMASK.reg |= PM_APBCMASK_ADC;

  ADC->REFCTRL.reg = 0x80;                         // Enable Ref Comp and use (INT1V) 1.0V voltage reference
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV512 |    // Divide Clock ADC GCLK by 512 (48MHz/512 = 93.7kHz)
                   ADC_CTRLB_RESSEL_16BIT; // |    // Set ADC resolution to 16 bits
                   //ADC_CTRLB_DIFFMODE;           // Set ADC to Differential Mode
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
  ADC->SAMPCTRL.reg = 0x0;                        // Set max Sampling Time Length to half divided ADC clock pulse (5.33us)
  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_1X |
                       ADC_INPUTCTRL_INPUTSCAN(0) | 
                       ADC_INPUTCTRL_INPUTOFFSET(0) |
                       ADC_INPUTCTRL_MUXNEG_GND |
                       ADC_INPUTCTRL_MUXPOS_PIN0;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1024 |   // Accumulate 256 samples for 16-bit resolution
                     ADC_AVGCTRL_ADJRES(0);        // Zero result adjust
  
}

void ADCSample(uint16_t& A0_result, uint16_t& A1_result, uint16_t& A2_result) 
{   
  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_1X |
                       ADC_INPUTCTRL_INPUTSCAN(0) | 
                       ADC_INPUTCTRL_INPUTOFFSET(0) |
                       ADC_INPUTCTRL_MUXNEG_GND |
                       ADC_INPUTCTRL_MUXPOS_PIN0;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->CTRLA.bit.ENABLE = 0x01;                    // Enable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization 

  delay(1);

  ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization
  while(!ADC->INTFLAG.bit.RESRDY);                 // Wait for the conversion to complete
  A0_result = ADC->RESULT.reg;                     // Read the ADC result,  This Clears the result ready (RESRDY) interrupt flag

  ADC->CTRLA.bit.ENABLE = 0x00;                    // Disable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization 

  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_1X |
                       ADC_INPUTCTRL_INPUTSCAN(0) | 
                       ADC_INPUTCTRL_INPUTOFFSET(0) |
                       ADC_INPUTCTRL_MUXNEG_GND |
                       ADC_INPUTCTRL_MUXPOS_PIN1;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->CTRLA.bit.ENABLE = 0x01;                    // Enable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization 

  delay(1);

  ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization
  while(!ADC->INTFLAG.bit.RESRDY);                 // Wait for the conversion to complete
  A1_result = ADC->RESULT.reg;                     // Read the ADC result,  This Clears the result ready (RESRDY) interrupt flag

  ADC->CTRLA.bit.ENABLE = 0x00;                    // Disable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization 

  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_1X |
                       ADC_INPUTCTRL_INPUTSCAN(0) | 
                       ADC_INPUTCTRL_INPUTOFFSET(0) |
                       ADC_INPUTCTRL_MUXNEG_GND |
                       ADC_INPUTCTRL_MUXPOS_PIN4;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->CTRLA.bit.ENABLE = 0x01;                    // Enable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization 

  delay(1);

  ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization
  while(!ADC->INTFLAG.bit.RESRDY);                 // Wait for the conversion to complete
  A2_result = ADC->RESULT.reg;                     // Read the ADC result,  This Clears the result ready (RESRDY) interrupt flag

  ADC->CTRLA.bit.ENABLE = 0x00;                    // Disable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization 
}

void loop() 
{ 
  double voltage1 = 0.0;
  double voltage2 = 0.0;
  double voltage3 = 0.0;
  uint16_t A0_result = 0;
  uint16_t A1_result = 0;
  uint16_t A2_result = 0;

  ADCSample(A0_result, A1_result, A2_result);

  voltage1 = ((double)A0_result / 65536.0) * 19.4170403 * errorCorrect;
  voltage2 = ((double)A1_result / 65536.0) * 19.4453004 * errorCorrect;
  voltage3 = ((double)A2_result / 65536.0) * 17.0469798 * errorCorrect;

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

  if (voltage3 < 0.0)
  {
    voltage3 = 0.0;
  }
  else if (voltage3 > 20.0)
  {
    voltage3 = 99.9;
  }

  if(Serial)
  {
    Serial.print((A0_result / 65536.0),4);
    Serial.print(" ");
    Serial.print(voltage1);
    Serial.print(" : ");
    Serial.print((A1_result / 65536.0),4);
    Serial.print(" ");
    Serial.print(voltage2);
    Serial.print(" : ");
    Serial.print((A2_result / 65536.0),4);
    Serial.print(" ");
    Serial.println(voltage3);
  }

  //Display the house battery voltage
  uint8_t tens = (uint8_t)(voltage1 / 10.0);
  voltage1 -= (float)tens * 10.0;
  uint8_t ones = (uint8_t)voltage1;
  voltage1 -= (float)ones;
  voltage1 *= 10.0;
  uint8_t tenths = (uint8_t)voltage1;

  unsigned char digit = 0x30 + tens;
  if(v1_lastCharPos0 != digit)
  {
    tft1.drawChar2(18, 115, digit, v1_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK);
    v1_lastCharPos0 = digit;
  }

  digit = 0x30 + ones;
  if(v1_lastCharPos1 != digit)
  {
    tft1.drawChar2(86, 115, digit, v1_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK);
    v1_lastCharPos1 = digit;
  }

  digit = 0x30 + tenths;
  if(v1_lastCharPos2 != digit)
  {
    tft1.drawChar2(178, 115, digit, v1_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK);
    v1_lastCharPos2 = digit;
  }

  //Display in Next Dispaly
  tens = (uint8_t)(voltage2 / 10.0);
  voltage2 -= (float)tens * 10.0;
  ones = (uint8_t)voltage2;
  voltage2 -= (float)ones;
  voltage2 *= 10.0;
  tenths = (uint8_t)voltage2;

  digit = 0x30 + tens;
  if(v2_lastCharPos0 != digit)
  {
    tft2.drawChar2(x1, topY, digit, v2_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK);
    v2_lastCharPos0 = digit;
  }

  digit = 0x30 + ones;
  if(v2_lastCharPos1 != digit)
  {
    tft2.drawChar2(x2, topY, digit, v2_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK);
    v2_lastCharPos1 = digit;
  }

  digit = 0x30 + tenths;
  if(v2_lastCharPos2 != digit)
  {
    tft2.drawChar2(x4, topY, digit, v2_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK);
    v2_lastCharPos2 = digit;
  }

  tens = (uint8_t)(voltage3 / 10.0);
  voltage3 -= (float)tens * 10.0;
  ones = (uint8_t)voltage3;
  voltage3 -= (float)ones;
  voltage3 *= 10.0;
  tenths = (uint8_t)voltage3;

  digit = 0x30 + tens;
  if(v3_lastCharPos0 != digit)
  {
    tft2.drawChar2(x1, botY, digit, v3_lastCharPos0, ST77XX_WHITE, ST77XX_BLACK);
    v3_lastCharPos0 = digit;
  }

  digit = 0x30 + ones;
  if(v3_lastCharPos1 != digit)
  {
    tft2.drawChar2(x2, botY, digit, v3_lastCharPos1, ST77XX_WHITE, ST77XX_BLACK);
    v3_lastCharPos1 = digit;
  }

  digit = 0x30 + tenths;
  if(v3_lastCharPos2 != digit)
  {
    tft2.drawChar2(x4, botY, digit, v3_lastCharPos2, ST77XX_WHITE, ST77XX_BLACK);
    v3_lastCharPos2 = digit;
  }
  
  delay(50);
}
#endif