#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS  3 // M5 RGB LED has only 3 LEDs

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() 
{
  pixels.clear();   // Set all pixel colors to 'off'
  pixels.show();    // Send the updated pixel colors to the hardware.
  delay(2000);      //

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));   // LED 0 - Red
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));   // LED 1 - Green
  pixels.setPixelColor(2, pixels.Color(0, 0, 255));   // LED 2 - Blue
  pixels.show();                                      // Send the updated pixel colors to the hardware.

  delay(2000);

}
