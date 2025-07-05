// Pin Assignment
#define BUTTON 0                    // Grove - Button connects to Arduino Digital D0 Port
#define PIR    1                    // PIR Motion Sensor connects to Arduino Digital D1 Port
#define BUZZER 2                    // Grove - Buzzer connects to Arduino Digital D2 Port

unsigned int count = 0;
bool button_state = false;
bool button = false;
bool armed  = false;
bool alert  = false;
bool motion = false;
bool led    = false;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);   // Set LED_BUILTIN (Internally assigned) as digital output to drive the built-in LED.
    pinMode(BUTTON, INPUT);         // Set BUTTON (D0) as digital input
    pinMode(PIR, INPUT);            // Set PIR (D1) as digital input
    pinMode(BUZZER, OUTPUT);        // Set BUZZER (D2) as digital output
}

void loop()
{
    button = digitalRead(BUTTON);   // Read BUTTON (D0)
    motion = digitalRead(PIR);      // Read PIR (D1)

    Serial.print("Button: :");
    Serial.print(button);

    Serial.print(" Motion: :");
    Serial.println(motion);


    if(button != button_state)      // If button is pressed
    {
      button_state = button;        // 
      if(button == true)            //
      {
        armed = !armed;             // ARM / Dis-ARM Toggle
        alert = false;              // Alert Flag
        count = 0;                  // Buzzer counter
        led = 0;                    // LED indicator flag

        if(armed)                   // 
          Serial.println("System Armed");
        else
        {
          Serial.println("System Disarmed");
          digitalWrite(BUZZER, LOW);
        }
      }  
      delay(100);                   // Debouncing delay
    }

    if (armed == true && motion == true)
    {
      Serial.println("Alarm!");
      digitalWrite(BUZZER, HIGH);      
      alert = true;   
    }

    if (alert == true)
    {
      count = count + 1;
      if (count == 10)
      {
        count = 0;
        led = !led;
      }
    }
    
    digitalWrite(LED_BUILTIN, led);
    delay (100);
}

