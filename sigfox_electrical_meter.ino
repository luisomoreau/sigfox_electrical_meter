const char led = 13;    
const char sensor = 0;

float voltage = 0;     //get tension on the output of the sensor
float limit = 1.20;    //Value in volt, limit to get the electrical meter blinking led. You may need to adjust this values depending on the environment

// Delay between each data sending through sigfox network
unsigned long prevMillis = 0;
unsigned long interval =  3600000; // = 60mn*60s*1000ms send a message every hour

// Include librairies for the Akeru
#include <Akeru.h>
#include <SoftwareSerial.h>

// Structure that will store the data
typedef struct
{
  unsigned int consumption=0;    // electrical consumption
} Payload;

Payload p;
 
void setup()
{
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    // Init Akeru
    Akeru.begin();
}

void loop()
{
    voltage = (analogRead(sensor) * 5.0) / 1024;  // converting this value into voltage
    if(voltage >= limit)
    {
        digitalWrite(led, HIGH); //switch on the LED
        p.consumption++;     
    }
    else
    {
        digitalWrite(led, LOW); //Switch off the LED
    }
    // Debug
    Serial.print("Voltage = ");
    Serial.print(voltage);
    Serial.println(" V");
    Serial.print(" Consumption :");
    Serial.print(p.consumption);
    Serial.println(" W/h");

    if ((millis() - prevMillis) > interval)
    {
      Akeru.begin();
      // Saving current time for next loop
      prevMillis = millis();
      // Send data through sigfox
      Akeru.send(&p, sizeof(p));
      p.consumption=0;
    }
    Serial.print(" Interval :");
    Serial.println(millis() - prevMillis);
    delay(70);  // delay to read values
}
