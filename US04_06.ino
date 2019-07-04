#include <IRremote.h> //IRremote Bibliothek für IR Fernbedinung einbinden

int Infrarot = 11;  //ArduinoPin 11 für den Infrarotsensor festgelegt

IRrecv irrecv(Infrarot);  //Objekt für IRremote Initalisierung an Pin 11

decode_results Ergebnis;  //Speicherung des empfangenen Daten durch Infrarot unter "Ergebnis"

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second

  irrecv.enableIRIn();   //Initialisierung des Infrarotempfängers
}

void loop()
{
  if (irrecv.decode(&Ergebnis)) //Wenn Daten am Infrarotempfänger empfangen wurden, ...
  {
    Serial.println(Ergebnis.value, DEC);  //... sollen diese als Dezimalzahl am seriellen Monitor ausgegeben werden
  
    if (Ergebnis.value == 16724175) //... und wenn die Daten mit der Zahl "16724175" übereinstimmen (Taste "1" der Fernbedingung)
    {
      Serial.println("Taste 1"); //... die Nachricht "Taste 1" am seriellen Monitor ausgegeben werden
    }
  
    if (Ergebnis.value == 16718055) //... und wenn die Daten mit der Zahl "16718055" übereinstimmen (Taste "2" der Fernbedingung)
    {
      Serial.println("Taste 2"); //... die Nachricht "Taste 2" am seriellen Monitor ausgegeben werden
    }
  
    irrecv.resume();  //... soll der IR-Empfänger den nächsten Wert einlesen
  }
}
