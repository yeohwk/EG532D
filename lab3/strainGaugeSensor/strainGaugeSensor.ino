#define ZERO_BUTTON 4                           // Grove - Button connects to Arduino Digital D4 Port
#define VALID_OBJECT_WEIGHT    200              // Valid object weight = 200g

#include <Wire.h>
#include "rgb_lcd.h"
#include "UNIT_SCALES.h"

rgb_lcd lcd;                                    // Instantiate LCD
UNIT_SCALES scales;                             // Instantiate strain-gauge sensor
float weight = 0;
bool zero_button = false;

void setup() 
{
    Wire.begin();                               // Initialize I2C port     
    Serial.begin(115200);                       // Initialize Serial Port for debugging
    pinMode(ZERO_BUTTON, INPUT);                // Set ZERO_BUTTON (D4) as digital input
    lcd.begin(16, 2);                           // Initialize LCD
    lcd.setRGB(255, 255, 0);                    // Yellow LCD Back Light            
 
    while (!scales.begin(&Wire))                // Initialize strain-gauge sensor
    {
        Serial.println("scales connect error");
        delay(1000);
    }
    scales.setLEDColor(0xFFFF00);               // Set strain-gauge sensor built-in RGB LED colour to Yellow (0xRRGGBB)  
//  scales.setEmaFilter(32);                    // Set moving average filter size
}

void loop() 
{
    weight = scales.getWeight();                                    // Read weight value from strain-gauge sensor    
    zero_button = digitalRead(ZERO_BUTTON);                         // Read ZERO_BUTTON (D4)

    Serial.print("Weight (g): ");                                   // Print measured weight to serial port
    Serial.println(weight);                                         //
    lcd.clear();                                                    // Clear LCD
    lcd.setCursor(0, 0); lcd.print("W (g): "); lcd.print(weight);   // Print weight value to LCD

    if(zero_button == true)                                         // Check if ZERO BUTTON is pressed             
    {
        scales.setOffset();                                         // If pressed, perform zero-offset to strain-gauge sensor
        lcd.setRGB(255, 0, 0);                                      // Red LCD Back Light   
        lcd.setCursor(0, 1); lcd.print("Zero Offset.");
        scales.setLEDColor(0xFF0000);                               // Set strain-gauge sensor built-in RGB LED colour to Red (0xRRGGBB)            
    }
    else
    {
        if(weight >= VALID_OBJECT_WEIGHT)
        {
            lcd.setRGB(0, 255, 0);                                  // Green LCD Back Light                        
            lcd.setCursor(0, 1); lcd.print("Obj Detected!");        // 
            scales.setLEDColor(0x00FF00);                           // Set strain-gauge sensor built-in RGB LED colour to Green (0xRRGGBB)            
        }
        else
        {
            lcd.setRGB(255, 255, 0);                                // Yellow LCD Back Light      
            lcd.setCursor(0, 1); lcd.print("Obj Absent.");          //  
            scales.setLEDColor(0xFFFF00);                           // Set strain-gauge sensor built-in RGB LED colour to Yellow (0xRRGGBB)                      
        }
    }
    delay(200);
}