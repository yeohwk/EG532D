
// (IMPORTANT) Wiring of Arduino Board with SEEED Studio Stepper Driver board:
// D3 (Arduino) -> IN1 (SEEED Stepper Driver)
// D4 (Arduino) -> IN3 (SEEED Stepper Driver)
// D5 (Arduino) -> IN2 (SEEED Stepper Driver)
// D6 (Arduino) -> IN4 (SEEED Stepper Driver)
//

#define PROXIMITY_SENSOR 0                         // NPN Output Inductive Proximity Sensor connects to Arduino Digital D0 Port

#define HALFSTEP 8
#include <AccelStepper.h>
#include "rgb_lcd.h"

rgb_lcd lcd;                                       // Instantiate LCD  
AccelStepper stepper(HALFSTEP, 3, 4, 5, 6, true);  // Instantiate Stepper Motor

bool container_present = false;

void setup()
{
   Wire.begin();                                   // Initialiaze I2C bus
   Serial.begin(115200);                           // Initialize Serial Port for debugging
   pinMode(PROXIMITY_SENSOR, INPUT_PULLUP);        // Set PROXIMITY_SENSOR (D0) as digital input
   lcd.begin(16, 2);                               // Initialize LCD
   lcd.setRGB(0, 255, 0);                          // Green LCD Back Light           
   stepper.setMaxSpeed(1000.0);                    // Set max speed (RPM)
   stepper.setAcceleration(100.0);                 // Set acceleration
   stepper.setSpeed(200);                          // Set Initial speed
   stepper.setCurrentPosition(0);                  // Reset motor position     
   stepper.moveTo(20000);                          // Set end stepping value (Run to 20000 steps and stop!)
}

void loop()
{
   container_present = digitalRead(PROXIMITY_SENSOR); // Inductive proximity sensor detecting presence of a metal container
   if(container_present == HIGH)                      // Sensor indicates container is present
   {
      Serial.println("Container detected!");          // Debug message and LCD UI messages
      Serial.println("Filling starts.");              //
      lcd.clear();                                    // 
      lcd.setCursor(0, 0);                            //
      lcd.write("CTR Detected!");                     //
      lcd.setCursor(0, 1);                            //
      lcd.write("Filling starts.");                   //

      stepper.setCurrentPosition(0);                  // Reset motor position     
      stepper.moveTo(10000);                          // Set stepper for 10000 steps and stop.

      while(stepper.distanceToGo() > 0)               // Check stepper motor steps.  
      {                                               //
         stepper.run();                               // Running stepper motor for 10000 steps to dispense a fixed amount of liquid
      }                                               //   

      lcd.clear();                                    //
      lcd.setCursor(0, 0);                            //
      lcd.write("Cntr Detected!");                    //
      lcd.setCursor(0, 1);                            //
      lcd.write("Filling ends");                      //
      Serial.println("Filling ends.");                //
      while(digitalRead(PROXIMITY_SENSOR) == HIGH);   // At the end of the filling, sensor wait got container to be removed.   
   }
   else                                               // Sensor indicates container is absent
   {
//    Serial.println("Container Absent.");
      lcd.clear();                                    //
      lcd.setCursor(0, 0);                            //
      lcd.write("Cntr Absent.");                      //
   }
   delay(500);                                        // Delayed for 0.5 seconds and the process restarts.
}
