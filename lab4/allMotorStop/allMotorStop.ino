#include <SparkFunMiniMoto.h>
 
MiniMoto motor1(0xCA); // CH1 (DC Direct-drive motor)
MiniMoto motor2(0xC0); // CH2 (DC Geared motor)
 
void setup()
{
    Serial.begin(115200);
}
 
void loop()
{
    motor1.stop();
    motor2.stop();
    delay(1000);
} 