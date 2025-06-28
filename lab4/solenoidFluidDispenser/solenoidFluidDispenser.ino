#define PROXIMITY_SENSOR  0 // NPN Output Inductive Proximity Sensor connects to Arduino Digital D0 Port
#define SOLENOID          1 // Solenoid (or relay) connects to Arduino Digital D1 Port

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;                                          // Instantiate LCD
bool container_present = false;

void setup() 
{
  Wire.begin();                                       // Initialiaze I2C bus
  Serial.begin(115200);                               // Initialize Serial Port for debugging  
  pinMode(PROXIMITY_SENSOR, INPUT_PULLUP);            // Set PROXIMITY_SENSOR (D0) as digital input
  pinMode(SOLENOID, OUTPUT);
  lcd.begin(16, 2);                                   // Initialize LCD
  lcd.setRGB(0, 255, 0);                              // Green LCD Back Light          
}

void loop() 
{
  container_present = digitalRead(PROXIMITY_SENSOR);  // Read sensor (D0) to check if a metal container is present 
  if (container_present == true)                      // Check if metal container is present.                  
  {
    Serial.println("Container Present!");
    Serial.println("Dispensing Liquid.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Container Pres!");
    lcd.setCursor(0, 1);
    lcd.write("Dispensing.");
    digitalWrite(SOLENOID, HIGH);                     // If container is present, energize solenoid to dispense liquid for 5 second
    delay(5000);                     
    digitalWrite(SOLENOID, LOW);                      // After 5 second, de-energize solenoid to stop dispensing and delay for 2 seconds
    Serial.println("Dispensing Done.");    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Container Pres!");
    lcd.setCursor(0, 1);
    lcd.write("Dispensing Done.");
    delay(2000);    

    while(digitalRead(PROXIMITY_SENSOR) == HIGH);     // Wait container to be removed.
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Container Abs!");
  }

  delay(500);
}
