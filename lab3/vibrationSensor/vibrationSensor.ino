#define VIBRATION_TH    2               // Machine vibration threshold

#include <math.h>
#include <Wire.h>
#include <ADXL345.h>
#include "rgb_lcd.h"

rgb_lcd lcd;                            // Instantiate LCD
ADXL345 adxl;                           // Instantiate ADXL345 MEMs sensor         

double xyz[3];
double ax, ay, az, a_mag;

void setup() 
{
    Wire.begin();
    Serial.begin(9600);
    lcd.begin(16, 2);                   // Initialize LCD
    lcd.setRGB(0, 255, 0);              // Green LCD Back Light             
    adxl.powerOn();

    // ADXL345 Setup
    // Set activity/inactivity thresholds (0-255)
    adxl.setActivityThreshold(75);      // 62.5mg per increment
    adxl.setInactivityThreshold(75);    // 62.5mg per increment
    adxl.setTimeInactivity(10);         // Set inactive timeout

    // Monitoring of activity movement on these axes -> 1 = on; 0 = off
    adxl.setActivityX(0);
    adxl.setActivityY(0);
    adxl.setActivityZ(0);

    // Monitoring of inactivity movement on these axes -> 1 = on; 0 = off
    adxl.setInactivityX(0);
    adxl.setInactivityY(0);
    adxl.setInactivityZ(0);

    // Monitoring of tap movement on these axes -> 1 = on; 0 = off
    adxl.setTapDetectionOnX(0);
    adxl.setTapDetectionOnY(0);
    adxl.setTapDetectionOnZ(0);

    // Set values for tap, and double-tap (0-255)
    adxl.setTapThreshold(50);           // 62.5mg per increment
    adxl.setTapDuration(15);            // 625us per increment
    adxl.setDoubleTapLatency(80);       // 1.25ms per increment
    adxl.setDoubleTapWindow(200);       // 1.25ms per increment

    // Set freefall values (0-255)
    adxl.setFreeFallThreshold(7);       //(5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(45);       //(20 - 70) recommended - 5ms per increment

    // Setting all interrupts to take place on Int pin 1
    // Issue (bug) with Int pin 2 -> unable to reset it
    adxl.setInterruptMapping(ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN);

    // Register interrupt actions - 1 == on; 0 == off
    adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 0);
    adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 0);
    adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  0);
    adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   0);
    adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 0);
}

void loop() 
{
    adxl.getAcceleration(xyz);          // Read the acceleration (vibration) values
    ax = xyz[0];
    ay = xyz[1];
    az = xyz[2];

    a_mag = ax * ax + ay * ay + az *az; // Calculate vibration magnitude
    a_mag = sqrt(a_mag);                //
    
    Serial.print("X= ");   Serial.print(ax);    Serial.print("g");
    Serial.print(" Y= ");  Serial.print(ay);    Serial.print("g");
    Serial.print(" Z= ");  Serial.print(az);    Serial.print("g ");
    Serial.print("Mag= "); Serial.print(a_mag); Serial.println("g");

    lcd.clear();                        // 
    lcd.setCursor(0, 0);                //
    lcd.print("V_Mag = ");              //
    lcd.print(a_mag);                   // Print vibration magnitude to LCD
    lcd.print("g");                     //    

    if(a_mag >= VIBRATION_TH)
    {
        Serial.println("Warning: High machine vibration detected!");
        lcd.setCursor(0, 1);
        lcd.print("High Vibration!");
        lcd.setRGB(255, 0, 0);          // Red LCD Back Light             
    }
    else
    {
        lcd.setCursor(0, 1);
        lcd.print("Normal Vibration");
        lcd.setRGB(0, 255, 0);          // Green LCD Back Light             
    }
    delay(200);
}
