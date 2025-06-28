// An ultrasonic distance sensor measuring fluid level in a large tank. 
// Buzzer triggers when fluid level is low (distance >= 30cm)

#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"
#include "MedianFilterLib.h"
#define ULTRASONIC_RANGER 1                           // Grove - Ultrasonic Ranger connects to Arduino D3 Port
#define BUZZER            2                           // Grove - Buzzer connects to Arduino Digital D2 Port
#define NUM_POINTS        5                           // Filter Size (number of measurement points)
#define DISTANCE_LOW_TH   30                          // 30cm distance low threshold (low fluid level)
#define DISTANCE_EMPTY_TH 50                          // 50cm distance empty threshold (tank empty)

rgb_lcd lcd;                                          // Initialize LCD
Ultrasonic ultrasonic(ULTRASONIC_RANGER);             // Initialize Grove - Ultrasonic Ranger
MedianFilter<float> medianFilter(NUM_POINTS);         // Initialize Median Filter.

void setup()
{
    pinMode(BUZZER, OUTPUT);                          // Set BUZZER (D2) as digital output
    Serial.begin(115200);                             // Initialize Serial Port
    lcd.begin(16, 2);                                 // Init LCD
    lcd.setRGB(0, 255, 0);                            // Green LCD Back Light    
}

void loop()
{
  float RangeInInches = 0;
  float RangeInCentimeters = 0;
  
  // Read distance value from sensor and feed into median filter
  RangeInInches = medianFilter.AddValue(ultrasonic.MeasureInInches());
  // Converts from Inch to CM
  RangeInCentimeters = RangeInInches * (float) 2.54;        

  // Prints result to Serial Monitor
  Serial.println("The distance to a target obstacle: ");
  Serial.print(RangeInInches);      // 0~157 inches
  Serial.println(" inch");
  Serial.print(RangeInCentimeters); // 0~400cm
  Serial.println(" cm");

  // Prints result to LCD
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("[in]: "); lcd.print(RangeInInches);
  lcd.setCursor(0, 1); lcd.print("[cm]: "); lcd.print(RangeInCentimeters);


  if (RangeInCentimeters >= DISTANCE_LOW_TH && RangeInCentimeters < DISTANCE_EMPTY_TH)
  {
    digitalWrite(BUZZER, HIGH);     // Fluid level is low, BUZZER (D2) is set to HIGH
    lcd.setRGB(255, 255, 0);        // Yellow LCD Back Light    
  }
  else if (RangeInCentimeters >= DISTANCE_EMPTY_TH)
  {
    digitalWrite(BUZZER, HIGH);     // Fluid level is acceptable, BUZZER (D2) is set to LOW  
    lcd.setRGB(255, 0, 0);          // Green LCD Back Light      
  }
  else
  {
    digitalWrite(BUZZER, LOW);      // Fluid level is acceptable, BUZZER (D2) is set to LOW  
    lcd.setRGB(0, 255, 0);          // Green LCD Back Light          
  }
  delay(100);
}
