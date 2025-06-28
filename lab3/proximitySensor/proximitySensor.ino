#define PROXIMITY_SENSOR 0                        // NPN Output Proximity Sensor connects to Arduino Digital D0 Port
#define BUZZER 2                                  // Grove - Buzzer connects to Arduino Digital D2 Port

#include <Wire.h>
#include "rgb_lcd.h"

// Initialize LCD
rgb_lcd lcd;

void setup()
{
    Serial.begin(115200);
    pinMode(PROXIMITY_SENSOR, INPUT_PULLUP);      // Set PROXIMITY_SENSOR (D0) as digital input
    pinMode(BUZZER, OUTPUT);                      // Set BUZZER (D2) as digital output
    lcd.begin(16, 2);                             // Init LCD
    lcd.setRGB(0, 255, 0);                        // Green LCD Back Light        
}

void loop()
{
    bool _sensor = digitalRead(PROXIMITY_SENSOR); // Read sensor (D0)

    if(_sensor == LOW)                            // Check if proximity sensor is asserted.
    {
      digitalWrite(BUZZER, HIGH);                 // If asserted, BUZZER (D2) is set to HIGH
      lcd.setRGB(255, 0, 0);                      // Red LCD Back Light 
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Object Det!");       
    }
    else
    {
      digitalWrite(BUZZER, LOW);                  // BUZZER (D2) is set to LOW if not asserted
      lcd.setRGB(0, 255, 0);                      // Green LCD Back Light
      lcd.clear(); 
      lcd.setCursor(0, 0); lcd.print("All Clear.");  
    }
    Serial.print("SENSOR = ");
    Serial.println(_sensor);
    delay(100);
}

