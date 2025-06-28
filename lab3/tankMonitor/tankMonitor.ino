#define ULTRASONIC_RANGER 1                           // Grove - Ultrasonic Ranger connects to Arduino D3 Port
#define BUZZER            2                           // Grove - Buzzer connects to Arduino Digital D2 Port
#define NUM_POINTS        5                           // Filter Size (number of measurement points)
#define TANK_FUL_TH       10                          // 10cm distance threshold for full fluid level
#define TANK_MED_TH       20                          // 20cm distance threshold for medium fluid empty
#define CO2_TH            1500                        // CO2 concentration threshold

#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"
#include "SCD30.h"
#include "MedianFilterLib.h"

rgb_lcd lcd;                                          // Instantiate LCD
Ultrasonic ultrasonic(ULTRASONIC_RANGER);             // Instantiate Grove - Ultrasonic Ranger
MedianFilter<float> medianFilter(NUM_POINTS);         // Instantiate Median Filter.

float RangeInInches = 0;
float RangeInCentimeters = 0;
float result[3] = {0};
float CO2;

void setup()
{
    pinMode(BUZZER, OUTPUT);                          // Set BUZZER (D2) as digital output
    Serial.begin(115200);                             // Initialize Serial Port
    lcd.begin(16, 2);                                 // Initialize LCD
    lcd.setRGB(0, 255, 0);                            // Green LCD Back Light
    scd30.initialize();                               // Initialize environmental sensor (Grove – CO2, Temperature, and Humidity Sensor)     
}

void loop()
{  
  // Read distance value from sensor and feed into median filter
  RangeInInches = medianFilter.AddValue(ultrasonic.MeasureInInches());
  // Converts from Inch to CM
  RangeInCentimeters = RangeInInches * (float) 2.54;

  if (scd30.isAvailable()) 
  {
      scd30.getCarbonDioxideConcentration(result);
      CO2 = result[0];
  }          

  // Prints result to Serial Monitor
  Serial.print("Tank Level: ");
  Serial.print(RangeInCentimeters); // 0~400cm
  Serial.print("cm ");
  Serial.print("CO2 (PPM): ");
  Serial.println(CO2);

  lcd.clear();

  // Tank Full Level
  if (RangeInCentimeters < TANK_FUL_TH )                                          
  {
 
  }
  // Tank Medium Level
  else if ( )
  {

  }
  // Tank Low Level
  else
  {

  }
  delay(100);
}
