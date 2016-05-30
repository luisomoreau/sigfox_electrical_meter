const char led = 13;    
const char sensor = 0;

float tension = 0;             //get tension on the output of the sensor
float seuilObscurite = 0.80;    //Value in volt, limit to get the electrical meter blinking led

// Delay between each data sending through sigfox network
unsigned long prevMillis = 0;
unsigned long interval =  3600000; // = 60mn*60s*1000ms

// Include librairies for the Akeru
#include <Akeru.h>
#include <SoftwareSerial.h>

// Structure that will store the data
typedef struct
{
  int consumption=0;    // conso electrique
} Payload;

Payload p;
 
void setup()
{
    //définition des broches utilisées
    pinMode(led, OUTPUT);
 
    Serial.begin(9600); //la voie série pour monitorer

    // Initialisation Akeru
    Akeru.begin();
}

void loop()
{
    
    tension = (analogRead(capteur) * 5.0) / 1024;  // converting this value into voltage
 
    if(tension >= seuilObscurite)
    {
        digitalWrite(led, HIGH); //switch on the LED
        p.consoelec++;
        
    }
    else
    {
        digitalWrite(led, LOW); //Switch off the LED
    }
    // Debug
    Serial.print("Voltage = ");
    Serial.print(tension);
    Serial.println(" V");
    Serial.print(" Consumption :");
    Serial.print(p.consoelec);
    Serial.println(" W/h");

    if ((millis() - prevMillis) > interval)
    {
      Akeru.begin();
      // On enregistre l'heure courante pour le prochain tour de loop
      // Saving current time for next loop
      prevMillis = millis();
      // Send data through sigfox
      Akeru.send(&p, sizeof(p));
      p.consoelec=0;
    }
    Serial.print(" Interval :");
    Serial.println(millis() - prevMillis);
   
 
    delay(70);  // delay to read values
}
