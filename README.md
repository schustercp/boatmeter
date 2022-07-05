# boatmeter
Voltage and current Meter for DC Panel

Based on the Adafruit QT Py - SAMD21 Dev Board driving 2 Displays (Adafruit 1.69" 280x240 Round Rectangle Color IPS TFT Display - ST7789)

The TrueType to Adafruit_GFX font converter was used to generate a Font that did not look blocky.

There was an inital problem when the display was redrawn the entire thing was blacked out and then drawn.  The slow dipslay interface made changing numbers on the displays very visualy unapealing.  The library was changed to only change the pixels that need to be changed.  The drawback is that the last digit displayed must be saved and passed back in.
