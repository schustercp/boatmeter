# boatmeter
Voltage and current Meter for DC Panel

Based on the Adafruit QT Py - SAMD21 Dev Board driving 2 Displays (Adafruit 1.69" 280x240 Round Rectangle Color IPS TFT Display - ST7789)

The TrueType to Adafruit_GFX font converter was used to generate a Font that did not look blocky.

There was an inital problem when the display was redrawn the entire thing was blacked out and then drawn.  The slow dipslay interface made changing numbers on the displays very visualy unapealing.  The library was changed to only change the pixels that need to be changed.  The drawback is that the last digit displayed must be saved and passed back in.

ADC - Registers
12 Bit Samples - Upsampled to 16 Bits

Clocks:
  CLK_APB_ADCx
  GCLK_ADC in System Controller (SYSCTRL)

Inputs: (Single Ended)
  ADC0
  ADC1
  
  The ADC can be enabled by writing a one to the Enable bit in the Control Register A (CTRLA.ENABLE).
  
CTRLA (8 Bits)
    ADC->CTRLA.bit.ENABLE
    ADC->CTRLA.bit.RUNSTDBY = 0
    ADC->CTRLA.bit.SWRST Write 1 to reset all ADC Registers
    
REFCTRL (8 Bits)
    //This sets REFCOMP and uses (INT1V) 1.0V voltage reference
    ADC->REFCTRL.reg = 0x80.
    
AVGCTRL (8 Bits)
    // Accumulate 256 samples for 16-bit resolution
    // Zero result adjust
    ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_256 | ADC_AVGCTRL_ADJRES(0);
    
SAMPCTRL (8 Bits)
  // Set max Sampling Time Length to half divided ADC clock pulse (5.33us)
  ADC->SAMPCTRL.reg = 0x00;
  
CTRLB (16 Bits)
  // Divide Clock ADC GCLK by 512 (48MHz/512 = 93.7kHz)
  // Set ADC resolution to 16 bits
  // CORREN = 0 Digital Correction Disabled
  // FREERUN = 0, The ADC run is single conversion mode
  // LEFTADJ = 0, The ADC conversion result is right-adjusted in the RESULT register.
  // DIFFMODE = 0, The ADC is running in singled-ended mode.
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV512 | ADC_CTRLB_RESSEL_16BIT;
  
WINCTRL (8 Bits)
  // No window mode (default)
  ADC->WINCTRL.reg = 0
  
SWTRIG (8 Bits)
  ADC->SWTRIG.bit.START = 1; //Starts an ADC Conversion
  ADC->SWTRIG.bit.FLUSH = 1; //Clears ADC pipeline
  
INPUTCTRL (32 Bits)
  //0 Gain
  //0 Input Offset
  //1 Input Scan
  //Mux Neg = 0
  //Mux Pos = 0
  ADC->INPUTCTRL.reg = 0x00010000;
  
EVCTRL (8 Bits)
  Defaults to 0
  
INTENCLR (8 Bits)
    //Write to clear interupts

INTENSET (8 Bits)
    //Interrupt Enable Bits
    Defaults all Off
    
INTFLAG (8 Bits)
    //Signaled Interrupts
    
STATUS (8 Bits)
    //Only one bit SYNCBUSY
    
Result (16 Bits)
WINLT (16 Bits) Window Lower Threashold, Not used
WINUT (16 Bits) Window Upper Threashold, Not used
GAINCORR (16 Bits) Not used
OFFSETCORR (16 Bits) Not Used
CALIB (16 Bits) Not used
DBGCTRL (8 Bits) Not used

  
