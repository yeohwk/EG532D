#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"

#define TEMP_SENSOR  A1       // Grove - Temperature Sensor connects to Analog Port A1
#define TEMP_TH      30       // Temperature Threshold
const int B  = 4275;          // B value of the thermistor (from Datasheet)
const int R0 = 100000;        // R0 = 100k
 
// Initialize LCD
rgb_lcd lcd;

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);         // Init LCD
    lcd.setRGB(0, 255, 0);    // Green LCD Back Light    
    analogReadResolution(12); // Initialize Analog Resolution to 12 bits.
}

void loop()
{
    int a = analogRead(TEMP_SENSOR);
    float R = 4095.0/a - 1.0;
    R = R0 * R;

    // converts R to temperature using simplified Steinhart–Hart equation
    float temperature = 1.0/(log(R/R0)/B + 1/298.15) - 273.15; 
    Serial.print("temperature = ");
    Serial.println(temperature);

    lcd.clear();
    if (temperature > TEMP_TH)
    {
      lcd.setRGB(255, 0, 0);  // Red LCD Back Light 
      lcd.setCursor(0, 0); lcd.print("Warning!");      
    }
    else
    {
      lcd.setRGB(0, 255, 0);  // Green LCD Back Light  
      lcd.setCursor(0, 0); lcd.print("Normal");    
    }

    lcd.setCursor(0, 1); lcd.print("TEMP(C): "); lcd.print(temperature);    
    delay(500);
}
