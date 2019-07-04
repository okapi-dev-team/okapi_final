#include <Stepper.h>  //Stepper Bibliothek für Schrittmotor einbinden

int SPU = 2048;  //Schritte pro Umdrehung des Schrittmotors auf 2048 festgelegt

Stepper Motor(SPU, 3,5,4,6);  //Objekt für Stepper Initialisierung mit Bezeichnung "Motor" und Angabe der Anschluss-Pins

}

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  
  Motor.setSpeed(10); //Festlegen der Geschwindigkeit des Schrittmotors auf 10 Umdrehungen pro Minute
}

void loop()
{
  Motor.step(2048);   //Der Schrittmotor macht 2048 Schritte (eine Umdrehung)
  
  delay(1000);  //Der Schrittmotor bleibt für eine Sekunde stehen
  
  Motor.step(-2048);    //Der Schrittmotor macht 2048 Schritte in die andere Richtung (eine Umdrehung)
  
  delay(1000);  //Der Schrittmotor bleibt für eine Sekunde stehen
}
