#define CO2_TH      1500                                        // CO2 concentration threshold
#define ATEMP_TH    30                                          // Ambient Temperature threshold
#define HUMIDITY_TH 60                                          // Humidity threshold

#include <Wire.h>
#include "rgb_lcd.h"
#include "SCD30.h"

rgb_lcd lcd;                                                    // Instantiate LCD
float result[3] = {0};

void setup() 
{
    Wire.begin();                                               // Initialize I2C Communication
    Serial.begin(115200);                                       // Initialize Serial Port
    lcd.begin(16, 2);                                           // Initialize LCD
    lcd.setRGB(0, 255, 0);                                      // Green LCD Back Light    
    scd30.initialize();                                         // Initialize environmental sensor (Grove – CO2, Temperature, and Humidity Sensor) 
}

void loop() 
{
    if (scd30.isAvailable()) 
    {
        scd30.getCarbonDioxideConcentration(result);
        Serial.print("CO2 (PPM): ");
        Serial.print(result[0]);
        Serial.print(" ATemp: ");
        Serial.print(result[1]);
        Serial.print("℃ ");
        Serial.print("Humidity: ");
        Serial.print(result[2]);
        Serial.println("%");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CO2: "); lcd.print(result[0]); lcd.print("PPM");          
        lcd.setCursor(0, 1); 
        lcd.print("T:");    lcd.print(result[1]); lcd.print(" H:");
        lcd.print(result[2]);

        if (result[0] >= CO2_TH)        // Check of CO2 concentration is above the threshold
        {
            lcd.setRGB(255, 0, 0);      // Red LCD Back Light if above threshold            
        }
        else
        {
            lcd.setRGB(0, 255, 0);      // Green LCD Back Light if below threshold           
        }
    }
    delay(1000);
}
