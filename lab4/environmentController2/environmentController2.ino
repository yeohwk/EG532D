#define CO2_TH            1500                                  // CO2 concentration threshold
#define ATEMP_TH          30                                    // Ambient Temperature threshold
#define HUMIDITY_TH       60                                    // Humidity threshold

#define RELAY1            0
#define RELAY2            1
#define RELAY3            2
#define RELAY4            3
#define ON                1
#define OFF               0

#include <Wire.h>
#include "rgb_lcd.h"
#include "SCD30.h"
#include "Unit_4RELAY.h"
 
rgb_lcd lcd;                                                    // Instantiate LCD
UNIT_4RELAY relay;                                              // Instantiate 4RELAY
float result[3] = {0};

void setup() 
{
    Wire.begin();                                               // Initialize I2C Communication
    Serial.begin(115200);                                       // Initialize Serial Port
    lcd.begin(16, 2);                                           // Initialize LCD
    lcd.setRGB(0, 255, 0);                                      // Green LCD Back Light    
    scd30.initialize();                                         // Initialize environmental sensor (Grove – CO2, Temperature, and Humidity Sensor) 

    relay.begin(&Wire);                                         // Initialize 4RELAY
    relay.Init(1);                                              // Set relay indicator to be synchronous
    relay.relayAll(0);                                          // Clear (OFF) all relays.
    delay(100);
    relay.relayWrite(RELAY1, ON);                               // Turns on primary exhaust fan (small, low-power suction fan);    
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

        if (result[0] >= CO2_TH)                    // Check if CO2 concentration is above the threshold
        {
            lcd.setRGB(255, 0, 0);                  // Red LCD Back Light if above threshold
            relay.relayWrite(RELAY2, ON);           // Turns on secondary exhaust fan (large, high power suction fan)
            delay(100);
            relay.relayWrite(RELAY3, ON);           // Turns on air circulation fan
            delay(100);
            relay.relayWrite(RELAY4, ON);           // Turns on warning beacon 
            delay(100);
            relay.relayWrite(RELAY1, OFF);          // Turns off primary exhaust fan (No longer needed for now)
        }
        else
        {
            lcd.setRGB(0, 255, 0);                  // Green LCD Back Light if below threshold
            relay.relayWrite(RELAY1, ON);           // Turns on promary exhaust fan (use low power fan as air quality is back to normal)
            delay(100);
            relay.relayWrite(RELAY2, OFF);          // Turns off secondary exhaust fan
            delay(100);
            relay.relayWrite(RELAY3, OFF);          // Turns off air circulation fan
            delay(100);
            relay.relayWrite(RELAY4, OFF);          // Turns off warning beacon 
        }
    }
    delay(1000);
}
