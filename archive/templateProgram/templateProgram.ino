#define PROXIMITY_SENSOR  0                           // NPN Output Inductive Proximity Sensor connects to Arduino Digital D0 Port
#define RELAY1            0
#define RELAY2            1
#define RELAY3            2
#define RELAY4            3
#define ON                1
#define OFF               0


#include <Wire.h>
#include "rgb_lcd.h"
#include "Unit_4RELAY.h"

rgb_lcd lcd;                                          // Instantiate LCD
UNIT_4RELAY relay;                                    // Instantiate 4RELAY
bool container_present = false;

void setup() 
{
  Wire.begin();                                       // Initialiaze I2C bus
  Serial.begin(115200);                               // Initialize Serial Port for debugging  
  pinMode(PROXIMITY_SENSOR, INPUT_PULLUP);            // Set PROXIMITY_SENSOR (D0) as digital input
  lcd.begin(16, 2);                                   // Initialize LCD
  lcd.setRGB(0, 255, 0);                              // Green LCD Back Light   

  relay.begin(&Wire);                                 // Initialize 4RELAY
  relay.Init(1);                                      // Set relay indicator to be synchronous
  relay.relayAll(0);                                  // Clear (OFF) all relays.
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
    relay.relayWrite(RELAY1, ON);                    	// If container is present, energize Relay1 to dispense liquid for 5 second
    delay(5000);                     
    relay.relayWrite(RELAY1, OFF);                      // After 5 second, de-energize Relay1 to stop dispensing and delay for 2 seconds
    Serial.println("Dispensing Done.");    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Container Pres!");
    lcd.setCursor(0, 1);
    lcd.write("Dispensing Done.");
    delay(2000);    

    while(digitalRead(PROXIMITY_SENSOR) == HIGH);     	// Wait container to be removed.
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Container Abs!");
  }

  delay(500);
}
