
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
   stepper.setSpeed(700);                          // Set stepper motor speed to 700RPM
}

void loop()
{  
   stepper.runSpeed();                             // Run stepper motor at defined speed (700RPM)
}
