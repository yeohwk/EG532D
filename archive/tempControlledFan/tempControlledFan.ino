#include <SparkFunMiniMoto.h>  // Include the MiniMoto library
 
// Create two MiniMoto instances, with different address settings.
MiniMoto motor1(0xCA); // CH1 (DC Direct-drive motor)
MiniMoto motor2(0xC0); // CH2 (DC Geared motor)
 
#define FAULTn  16     // Pin used for fault detection.
 
// Nothing terribly special in the setup() function- prep the
//  serial port, print a little greeting, and set up our fault
//  pin as an input.
void setup()
{
    Serial.begin(115200);
    Serial.println("Hello, world!");
    pinMode(FAULTn, INPUT);
}
 
// The loop() function just spins the motors one way, then the
//  other, while constantly monitoring for any fault conditions
//  to occur. If a fault does occur, it will be reported over
//  the serial port, and then operation continues.
void loop()
{
    Serial.println("Forward!");
    motor1.stop();
    motor2.stop();

//    motor0.brake();
//    motor1.brake();



//    motor0.drive(-100);
//    motor1.drive(100);
/*  delayUntil(1000);
    Serial.println("Stop!");
    motor0.stop();
    motor1.stop();
    delay(1000);
    Serial.println("Reverse!");
    motor0.drive(-100);
    motor1.drive(-100);
    delayUntil(1000);
    Serial.println("Brake!");
    motor0.brake();
    motor1.brake();*/
    delay(1000);
}
 
// delayUntil() is a little function to run the motor either for
//  a designated time OR until a fault occurs. Note that this is
//  a very simple demonstration; ideally, an interrupt would be
//  used to service faults rather than blocking the application
//  during motion and polling for faults.
void delayUntil(unsigned long elapsedTime)
{
    // See the "BlinkWithoutDelay" example for more details on how
    //  and why this loop works the way it does.
    unsigned long startTime = millis();
    while (startTime + elapsedTime > millis())
    {
        // If FAULTn goes low, a fault condition *may* exist. To be
        //  sure, we'll need to check the FAULT bit.
        if (digitalRead(FAULTn) == LOW)
        {
            // We're going to check both motors; the logic is the same
            //  for each...
            byte result = motor1.getFault();
            // If result masked by FAULT is non-zero, we've got a fault
            //  condition, and we should report it.
            if (result & FAULT)
            {
                Serial.print("Motor 0 fault: ");
                if (result & OCP) Serial.println("Chip overcurrent!");
                if (result & ILIMIT) Serial.println("Load current limit!");
                if (result & UVLO) Serial.println("Undervoltage!");
                if (result & OTS) Serial.println("Over temp!");
                break; // We want to break out of the motion immediately,
                //  so we can stop motion in response to our fault.
            }
            result = motor1.getFault();
            if (result & FAULT)
            {
                Serial.print("Motor 1 fault: ");
                if (result & OCP) Serial.println("Chip overcurrent!");
                if (result & ILIMIT) Serial.println("Load current limit!");
                if (result & UVLO) Serial.println("Undervoltage!");
                if (result & OTS) Serial.println("Over temp!");
                break;
            }
        }
    }
}