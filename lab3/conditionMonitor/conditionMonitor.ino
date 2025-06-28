#define TEMP_THRESHOLD    40            // Max Temperature Threshold (degree C)
#define VIBR_THRESHOLD    2.0           // Vibration Threshold (g)

#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Seeed_AMG8833_driver.h"
#include "ADXL345.h"

rgb_lcd lcd;                            // Instantiate LCD instance
AMG8833 tsa(0x69);                      // Instantiate AMG8833 instance of the  AMG8833 TSA library (use 0x68 or ox69)
ADXL345 adxl;                           // Instantiate ADXL345 instance of the ADXL345 Accelerometer library

unsigned int maxTempIndex = 0;          // Index of Maximum Temperature on TSA 
float maxTemp;                          // Maximum Temperature
double xyz[3];                          // X, Y, Z acceleration values (g)
double xyzMag;                          // Acceleration magnitude |XYZ| 

void adxlInitialize()
{
    adxl.powerOn();

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
    adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(45);       // (20 - 70) recommended - 5ms per increment

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

float getMaximumTemperature(unsigned int* index)
{
    float pixelsTemperature[PIXEL_NUM] = {0};
    float maxTemp = 0;
    tsa.read_pixel_temperature(pixelsTemperature);                      // Read temperature of all the 64 pixels
    for(int i = 0; i < 64; i++)
    {
        if(pixelsTemperature[i] > maxTemp)
        {
            maxTemp = pixelsTemperature[i];
            *index = i;
        }
    }
    return maxTemp;    
}

void setup() 
{
    Serial.begin(115200);                                               // Initialize Serial Port 
    lcd.begin(16, 2);                                                   // Initialize LCD (LCD Start!)
    lcd.setRGB(0, 255, 0);                                              // Green LCD Back Light
    if (tsa.init()) {                                                   // Initialize AMG8833 TSA
        Serial.println("TSA Init Failed!!!");                           // If TSA initialize failed, Print Failed Message to Serial Port
        lcd.setRGB(255, 0, 0);                                          // Set Red LCD Back Light
        lcd.print("TSA FAILED!");                                       // Print Failed Message to LCD
        while (1);                                                      // Stop Here!
    }
    Serial.println("AMG8833 Init OK!!!");                               // If TSA initialize successfull, Print, Success Message to Serial Port
    adxlInitialize();                                                   // Initialize Accelerometer  
}

void loop() 
{
    maxTemp = getMaximumTemperature(&maxTempIndex);                     // Read Maximum non-contact Temperature and its Index from TSA    
    adxl.getAcceleration(xyz);                                          // Read acceleration values 
    xyzMag = (xyz[0] * xyz[0]) + (xyz[1] * xyz[1]) + (xyz[2] * xyz[2]); // Calculate |XYZ| acceleration magnitude 
    xyzMag = sqrt(xyzMag);                                              // Calculate |XYZ| acceleration magnitude 

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T(");  lcd.print(maxTemp);                               // Print machine temperature to LCD
    lcd.print(")V("); lcd.print(xyzMag);                                // Print machine vibration magnitude to LCD
    lcd.print("g)");    

    if(maxTemp > TEMP_THRESHOLD || xyzMag > VIBR_THRESHOLD)             // Implements simple thresholds checks for monitoring of machine health (Todo: Need Improvements!)
    {
        lcd.setCursor(0, 1);
        lcd.setRGB(255, 0, 0);                                          // Set Red LCD Back Light
        lcd.print("[W] Mac Check!");                                    // Print Warning Message to LCD when threshold exceeded
    }
    else
    {
        lcd.setCursor(0, 1);
        lcd.setRGB(0, 255, 0);                                          // Set Green LCD Back Light
        lcd.print("[I] Mac OK!");                                       // Print Information Message to LCD when thresholds are within range  
    }    

    // Todo: Send Sensor values to Cloud Dashboard.    

    // Print all sensor values to Serial Port.
    Serial.print("Machine Temp = "); 
    Serial.print(maxTemp);
    Serial.print(" Vibration Magnitude = ");
    Serial.print(xyzMag);
    Serial.println("g");
    delay(200);            
}