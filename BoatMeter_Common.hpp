
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789

union FloatBytes 
{
    float f;
    unsigned char bytes[sizeof(float)];
};

struct DATA_LINE
{
  unsigned char lastCharPos[6];

  FloatBytes Value;

  int y;

  int offset;
  int x[6];

  int beforeDec;
  int afterDec;
};

float p = 3.1415926;

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:
//Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst);
Adafruit_ST7789 tft = Adafruit_ST7789(0, 2, 1);

void Update(DATA_LINE& line);
