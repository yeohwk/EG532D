#define CO2_TH      1500                                        // CO2 concentration threshold
#define ATEMP_TH    30                                          // Ambient Temperature threshold
#define HUMIDITY_TH 60                                          // Humidity threshold

#include <Wire.h>
#include <SparkFunMiniMoto.h>
#include "rgb_lcd.h"
#include "SCD30.h"
 
// Create two MiniMoto instances, with different address settings.

rgb_lcd lcd;                                                    // Instantiate LCD
MiniMoto motor1(0xCA);                                          // Instantiate MiniMotor CH1 (DC Direct-drive motor)
MiniMoto motor2(0xC0);                                          // Instantiate MiniMotor CH2 (Spare)
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

        if (result[0] >= CO2_TH)        // Check if CO2 concentration is above the threshold
        {
            lcd.setRGB(255, 0, 0);      // Red LCD Back Light if above threshold
            motor1.drive(-100);         // Run exhaust fan motor at maximum speed in one direction
                                        // Try changing the value of -100 to 100, what do you observe ?           
        }
        else
        {
            lcd.setRGB(0, 255, 0);      // Green LCD Back Light if below threshold
            motor1.drive(-25);          // Run exhaust fan motor at low speed in one direction
//          motor1.stop();              // Motor can also be stopped when CO2 concentration drops to below threshold  
        }
    }
    delay(1000);
}
