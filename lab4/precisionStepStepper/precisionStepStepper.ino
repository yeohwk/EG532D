
// (IMPORTANT) Wiring of Arduino Board with SEEED Studio Stepper Driver board:
// D3 (Arduino) -> IN1 (SEEED Stepper Driver)
// D4 (Arduino) -> IN3 (SEEED Stepper Driver)
// D5 (Arduino) -> IN2 (SEEED Stepper Driver)
// D6 (Arduino) -> IN4 (SEEED Stepper Driver)
//

#define HALFSTEP 8
#include <AccelStepper.h>

AccelStepper stepper(HALFSTEP, 3, 4, 5, 6, true);  // Instantiate Stepper Motor

void setup()
{  
   stepper.setMaxSpeed(1000.0);                    // Set max speed (RPM)
   stepper.setAcceleration(100.0);                 // Set acceleration
   stepper.setSpeed(200);                          // Set Initial speed
   stepper.setCurrentPosition(0);                  // Reset motor position     
   stepper.moveTo(20000);                          // Set end stepping value (Run to 20000 steps and stop!)
}

void loop()
{
  //Change direction when the stepper reaches the target position
   Serial.println(stepper.distanceToGo());
   if (stepper.distanceToGo() == 0)
   {
      stepper.moveTo(-stepper.currentPosition());
   }     
   stepper.run();                                  // Run stepper motor at defined speed (700RPM)
}
