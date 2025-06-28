#include <Wire.h>
#include "rgb_lcd.h"
#include "Adafruit_TCS34725.h"

float red, green, blue;
uint32_t colorcode;

rgb_lcd lcd;                                                                                // Instantiate LCD
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // Instantiate TC34725 colour sensor

void setup()
{
    Wire.begin();               // Initialize I2C port     
    Serial.begin(115200);       // Initialize Serial Port for debugging
    lcd.begin(16, 2);           // Initialize LCD
    lcd.setRGB(0, 255, 0);      // Green LCD Back Light                

    while (!tcs.begin())        // Initialize TC34725 colour sensor     
    { 
        Serial.println("TCS34725 not found ... check your connections");
        delay(1000);
    }
    tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_614MS);     // Sets the integration time for the TC34725 colour sensor TCS34725_INTEGRATIONTIME_154MS                                                            
    tcs.setGain(TCS34725_GAIN_16X);                             // Adjusts the gain on the TCS34725 colour sensor
}

void loop()
{
    tcs.getRGB(&red, &green, &blue);                                                // Reads Red, Green and Blue values.
    colorcode = (uint8_t) red * 65536 + (uint8_t) green * 256 + (uint8_t) blue;     // Format colorcode = 0xRRGGBB

    Serial.print("R (");                // Print colour values to Serial Port
    Serial.print((uint8_t) red);        //
    Serial.print(") G(");               //
    Serial.print((uint8_t) green);      //
    Serial.print(") B(");               //
    Serial.print((uint8_t) blue);       //    
    Serial.print(") Colorcode = 0x");   //    
    Serial.println(colorcode, HEX);     //

    lcd.clear();                        // Print colour values to LCD
    lcd.setCursor(0, 0);                //
    lcd.print("R(");                    //
    lcd.print((uint8_t) red);           //   
    lcd.print(") G(");                  //
    lcd.print((uint8_t) green);         //    
    lcd.print(")");                     //
    lcd.setCursor(0, 1);                //
    lcd.print("B(");                    //
    lcd.print((uint8_t) blue);          //
    lcd.print(") 0x");                  //
    lcd.print(colorcode, HEX);          //

    delay(800);
}