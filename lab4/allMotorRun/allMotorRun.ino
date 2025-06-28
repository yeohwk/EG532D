#include <SparkFunMiniMoto.h>
 
MiniMoto motor1(0xCA); // CH1 (DC Direct-drive motor)
MiniMoto motor2(0xC0); // CH2 (DC Geared motor)
 
void setup()
{
    Serial.begin(115200);
}
 
void loop()
{
    motor1.drive(100);
    motor2.drive(100);
    delay(1000);
} 