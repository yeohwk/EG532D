#define PROXIMITY_SENSOR 0          // NPN Output Inductive Proximity Sensor connects to Arduino Digital D0 Port
#define SERVO_MOTOR      1          // Servo motor connects to Arduino Digital D1 Port 
#define RED_COLOUR_TH    80         // Stamp (Servo motor) activates when red >= RED_COLOUR_TH

#include <Wire.h>
#include <SparkFunMiniMoto.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"
#include "rgb_lcd.h"

float red, green, blue;
uint32_t colorcode;
bool obj_present = false;

MiniMoto motor1(0xCA); // CH1 (Spare)
MiniMoto motor2(0xC0); // CH2 (Conveyor belt motor - DC Geared motor)

rgb_lcd lcd;                                                                                // Instantiate LCD
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // Instantiate TC34725 colour sensor
Servo servo;                                                                                // INstantite Servo motor

void setup() 
{
    Wire.begin();                                                                           // Initialiaze I2C bus
    Serial.begin(115200);                                                                   // Initialize Serial Port for debugging
    pinMode(PROXIMITY_SENSOR, INPUT_PULLUP);                                                // Set PROXIMITY_SENSOR (D0) as digital input
    servo.attach(SERVO_MOTOR);                                                              // Initialize Servo motor and attach to Arduino Digital D1 Port
    servo.write(0);                                                                         // Servo moves to Home (0 degree) 
    lcd.begin(16, 2);                                                                       // Initialize LCD
    lcd.setRGB(0, 255, 0);                                                                  // Green LCD Back Light        
    while (!tcs.begin())                                                                    // Initialize TC34725 colour sensor     
    { 
        Serial.println("TCS34725 not found ... check your connections");
        delay(1000);
    }
    tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_614MS);                                 // Sets the integration time for the TC34725 colour sensor TCS34725_INTEGRATIONTIME_154MS                                                            
    tcs.setGain(TCS34725_GAIN_16X);                                                         // Adjusts the gain on the TCS34725 colour sensor
    motor2.drive(50);                                                                       // Starts conveyor belt motor at 50% full speed.
}

void loop() 
{
    obj_present = digitalRead(PROXIMITY_SENSOR);                                            // Read sensor (D0)
    tcs.getRGB(&red, &green, &blue);                                                        // Reads Red, Green and Blue values.
    colorcode = (uint8_t) red * 65536 + (uint8_t) green * 256 + (uint8_t) blue;             // Format colorcode = 0xRRGGBB

    Serial.print("R (");                                                                    // 
    Serial.print((uint8_t) red);                                                            // Print red colour values to Serial Port
    Serial.print(") G(");                                                                   //
    Serial.print((uint8_t) green);                                                          // Print green colour values to Serial Port
    Serial.print(") B(");                                                                   //
    Serial.print((uint8_t) blue);                                                           // Print blue colour values to Serial Port   
    Serial.print(") Colorcode = 0x");                                                       //    
    Serial.println(colorcode, HEX);                                                         // Print colourcode

    lcd.clear();
    if(obj_present == HIGH)                                                                 // If inductive proximity sensor detects an object
    {
        Serial.println("Object Present!");
        lcd.setCursor(0, 0);
        lcd.write("Obj Present!");
        if(red > RED_COLOUR_TH)                                                             // If object colour red >= RED_COLOUR_TH 
        {
            lcd.setCursor(0, 1);
            lcd.write("Stamping.");
            servo.write(90);                                                                // Turn servo motor 90 degree to stamp object.
            delay(1000);
            servo.write(0);                                                                 // Servo moves to Home (0 degree) 
            while(digitalRead(PROXIMITY_SENSOR) == HIGH);     
        }
    }
    else
    {
        Serial.println("No Object!");
        lcd.setCursor(0, 0);
        lcd.write("No Obj!");         
    }
    delay(800);
}
