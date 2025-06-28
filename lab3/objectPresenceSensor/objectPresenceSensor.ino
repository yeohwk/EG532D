// Unit-ToF4M is a smart Laser ToF distance sensor. The sensor is use for non-contact precision distance measurement of a target object.
// The various potential usages of this ToF distance sensor:
// (a) Detect if a component is present at a specific workstation or assembly point
// (b) Trigger actions such as activating a robotic arm, conveyor belt, or inspection process when an object enters a predefined zone
// (c) Monitor occupancy or detect obstructions in automated guided vehicle (AGV) paths
//
// This example sketch performs object peesence detection. Take note that median filter is not required.
//

#define PRESENCE_TH             500     // In mm (50 cm)
#define PRESENCE_TOO_CLOSE_TH   100     // In mm (10 cm) 
#include <Wire.h>
#include <VL53L1X.h>
#include "rgb_lcd.h"

rgb_lcd lcd;                            // Instantiate LCD
VL53L1X sensor;                         // Instantiate Unit-ToF4M ToF sensor
unsigned int distance;

void setup() 
{
    Wire.begin();                       // Initialiaze I2C bus
    Serial.begin(115200);               // Initialize Serial Port for debugging
    lcd.begin(16, 2);                   // Initialize LCD
    lcd.setRGB(0, 255, 0);              // Green LCD Back Light        
    sensor.setBus(&Wire);               // Configure sensor to use the I2C bus. 
    sensor.setTimeout(500);             // Configure sensor timeout.
    if (!sensor.init()) {               // Initialize sensor
        Serial.println("Failed to detect and initialize sensor!");
        while (1);
    }

    // Use long distance mode and allow up to 50000 us (50 ms) for a
    // measurement. You can change these settings to adjust the performance of
    // the sensor, but the minimum timing budget is 20 ms for short distance
    // mode and 33 ms for medium and long distance modes. See the VL53L1X
    // datasheet for more information on range and timing limits.
    sensor.setDistanceMode(VL53L1X::Long);
    sensor.setMeasurementTimingBudget(50000);

    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    sensor.startContinuous(50);
}

void loop() {
    sensor.read();
    distance = sensor.ranging_data.range_mm;
    if(distance > PRESENCE_TOO_CLOSE_TH && distance < PRESENCE_TH)
    {
        Serial.println("Object Presence and within Valid Range.");
        lcd.clear();
        lcd.setRGB(255, 255, 0);                                        // Yellow LCD Back Light   
        lcd.setCursor(0, 0); lcd.print(distance); lcd.print(" mm");
        lcd.setCursor(0, 1); lcd.print("Obj Detected!");
    }
    else if (distance > 0 && distance <= PRESENCE_TOO_CLOSE_TH)
    {
        Serial.println("Object Presence and is Too CLose!");
        lcd.clear();
        lcd.setRGB(255, 0, 0);                                          // Red LCD Back Light   
        lcd.setCursor(0, 0); lcd.print(distance); lcd.print(" mm");
        lcd.setCursor(0, 1); lcd.print("Obj Too Close!");
    }
    else
    {
        lcd.clear();
        lcd.setRGB(0, 255, 0);                                          // Green LCD Back Light   
        lcd.setCursor(0, 0); lcd.print(distance); lcd.print(" mm");      
    }
}
